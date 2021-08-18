#include "chain.h"

std::string CTx::print(uint8_t n)
{
    std::string string;
    string =  dent(n) + "CTxOut{\n";
    string += dent(n+1) + "hash = " + StringToHexReversed(hash) + "\n";
    string += dent(n+1) + "blkHeight = " + std::to_string(blkHeight) + "\n";
    string += dent(n+1) + "nHeight = " + std::to_string(nHeight) + "\n";
    string += dent(n+1) + "isCoinBase = " + std::to_string(isCoinBase) + "\n";
    string += dent(n+1) + "amount = " + std::to_string(amount) + "\n";
    string += dent(n+1) + "scriptSig = " + StringToHex(scriptSig) + "\n";
    string += dent(n) + "}\n";
    return string;
}

Chain::Chain(const char* path, bool create_if_missing) : LevelDb::LevelDb(path, create_if_missing)
{
    cit = db->NewIterator(leveldb::ReadOptions());
    cit->SeekToFirst();
    assert(cit->Valid());
    // initialization steps
    getObfuscationKey();
    getBlockHash();
}

void Chain::getObfuscationKey()
{
    std::string key = cit->key().ToString();
    std::string value = cit->value().ToString();
    cit->Next();
    assert(cit->Valid());
    std::string key_pattern;
    key_pattern += char(0x0e);
    key_pattern += char(0x00);
    key_pattern += "obfuscate_key";
    assert(key == key_pattern);
    this->obfuscation_key = value.substr(1);
}

void Chain::getBlockHash()
{
    Row row = getRow();
    assert(*(char*)row.first.c_str() == 'B');
    this->hash = row.second;
}

uint64_t Chain::getLength(uint64_t length)
{
    if(length){return length;}
    // get current position
    std::string pos = cit->key().ToString();
    uint64_t counter = 0;
    while(cit->Valid())
    {
        cit->Next();
        counter++;
    }
    cit->Seek(leveldb::Slice(pos));
    // cit->SeekToFirst();
    // cit->Next();
    // cit->Prev();
    return counter;
}

std::string Chain::applyObfuscationKey(const std::string& data)
{
    char buffer[data.size()];
    for(size_t it = 0; it < data.size(); it++)
        buffer[it] = (char)((char)data[it] ^ (char)obfuscation_key[it % obfuscation_key.size()]);
    std::string ret(buffer, data.size());
    return ret;
}

Row Chain::getRow()
{
    std::string key;
    std::string value;
    while(true)
    {
        if(cit->Valid())
        {
            key = cit->key().ToString();
            value = cit->value().ToString();
            break;
        }
        else
        {
            cit->Next();
        }
    }
    cit->Next();
    Row ret(key, applyObfuscationKey(value));
    return ret;
}

CTx readCTx(Chain* chain)
{
    Row row = chain->getRow();
    CTx ctx;

    BStream bstream_key(&row.first);
    bstream_key.movePos(1);
    ctx.hash = bstream_key.read(32);
    ctx.nHeight = bstream_key.readVarInt();

    BStream bstream_value(&row.second);
    uint64_t code =  bstream_value.readVarInt();
    ctx.isCoinBase = (bool)(code & 1);
    ctx.blkHeight = code >> 1;
    ctx.amount = bstream_value.decompressAmount(bstream_value.readVarInt());
    ctx.scriptSig = bstream_value.read(bstream_value.readSpecialSize());
    return ctx;
}

std::string composeCTxHash(std::string& hash, uint32_t n)
{
    std::string ret;
    ret += 'C';
    ret += hash;
    ret += writeVarInt(n);
    return ret;
}

CTx fetchCTx(Chain* chain, std::string& cTxHash)
{
    leveldb::Slice key (cTxHash);
    std::string value;
    leveldb::Status status = chain->db->Get(leveldb::ReadOptions(), key, &value);
    value = chain->applyObfuscationKey(value);

    CTx ctx;

    BStream bstream_key(&cTxHash);
    bstream_key.movePos(1);
    ctx.hash = bstream_key.read(32);
    ctx.nHeight = bstream_key.readVarInt();

    BStream bstream_value(&value);
    uint64_t code =  bstream_value.readVarInt();
    ctx.isCoinBase = (bool)(code & 1);
    ctx.blkHeight = code >> 1;
    ctx.amount = bstream_value.decompressAmount(bstream_value.readVarInt());
    ctx.scriptSig = bstream_value.read(bstream_value.readSpecialSize());
    return ctx;
}