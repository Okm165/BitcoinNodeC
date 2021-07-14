#include "chain.h"

std::string CBlock::print(int n)
{
    std::string string;
    string =  dent(n) + "CTxOut{\n";
    string += dent(n+1) + "hash = " + StringToHexReversed(hash) + "\n";
    string += dent(n+1) + "blkHeight = " + std::to_string(blkHeight) + "\n";
    string += dent(n+1) + "nHeight = " + std::to_string(nHeight) + "\n";
    string += dent(n+1) + "isCoinBase = " + std::to_string(isCoinBase) + "\n";
    string += dent(n+1) + "amount = " + std::to_string(amount) + "\n";
    string += dent(n+1) + "scriptType = " + std::to_string(scriptType) + "\n";
    string += dent(n+1) + "address = " + address + "\n";
    string += dent(n) + "}\n";
    return string;
}

Chain::Chain(const char* path, uint64_t cursor, bool create_if_missing) : LevelDb::LevelDb(path, create_if_missing)
{
    this->cursor = cursor;
    it = db->NewIterator(leveldb::ReadOptions());
    it->SeekToFirst();
    assert(it->Valid());
    // initialization steps
    getObfuscationKey();
    getBlockHash();
}

void Chain::getObfuscationKey()
{
    std::string key = it->key().ToString();
    std::string value = it->value().ToString();
    cursor++;
    it->Next();
    assert(it->Valid());
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
    std::string key = it->key().ToString();
    std::string value = it->value().ToString();
    cursor++;
    it->Next();
    assert(it->Valid());
    Row ret(key, applyObfuscationKey(value));
    return ret;
}

CBlock readCBlock(Chain& chain, OpCodes& opcodes)
{
    Row row = chain.getRow();
    CBlock cblock;

    BStream bstream_key(row.first);
    bstream_key.movePos(1);
    cblock.hash = bstream_key.read(32);
    cblock.nHeight = bstream_key.readVarInt();

    BStream bstream_value(row.second);
    uint64_t code =  bstream_value.readVarInt();
    cblock.isCoinBase = (bool)(code & 1);
    cblock.blkHeight = code >> 1;
    cblock.amount = bstream_value.decompressAmount(bstream_value.readVarInt());
    cblock.scriptType = bstream_value.readVarInt();
    cblock.address = addressDecodeType(bstream_value, cblock.scriptType, opcodes);
    return cblock;
}