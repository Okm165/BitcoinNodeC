#include "address.h"

const char* GetAddressDecoderModeName(AddressDecoderMode mode)
{
    switch (mode)
    {
    case TYPE                       : return "TYPE";
    case SCRIPT                     : return "SCRIPT";
    default                         : return "MODE_UNKNOWN";
    }
}

const char* GetAddressDecoderTypeName(AddressDecoderType type)
{
    switch (type)
    {
    case NONE                       : return "NONE";
    case FAIL                       : return "FAIL";
    case P2SH                       : return "P2SH";
    case P2PKH                      : return "P2PKH";
    case P2WSH                      : return "P2WSH";
    case OTHER                      : return "OTHER";
    case P2WPKH                     : return "P2WPKH";
    case P2PKH_COMPRESSED           : return "P2PKH_COMPRESSED";
    case P2PKH_NO_RIPEMDSHA         : return "P2PKH_NO_RIPEMDSHA";
    default                         : return "TYPE_UNKNOWN";
    }
}

const char* GetOpName(OpCodeType opcode)
{
    switch (opcode)
    {
    // push value
    case OP_0                       : return "0";
    case OP_PUSHDATA1               : return "OP_PUSHDATA1";
    case OP_PUSHDATA2               : return "OP_PUSHDATA2";
    case OP_PUSHDATA4               : return "OP_PUSHDATA4";
    case OP_1NEGATE                 : return "-1";
    case OP_RESERVED                : return "OP_RESERVED";
    case OP_1                       : return "1";
    case OP_2                       : return "2";
    case OP_3                       : return "3";
    case OP_4                       : return "4";
    case OP_5                       : return "5";
    case OP_6                       : return "6";
    case OP_7                       : return "7";
    case OP_8                       : return "8";
    case OP_9                       : return "9";
    case OP_10                      : return "10";
    case OP_11                      : return "11";
    case OP_12                      : return "12";
    case OP_13                      : return "13";
    case OP_14                      : return "14";
    case OP_15                      : return "15";
    case OP_16                      : return "16";

    // control  
    case OP_NOP                     : return "OP_NOP";
    case OP_VER                     : return "OP_VER";
    case OP_IF                      : return "OP_IF";
    case OP_NOTIF                   : return "OP_NOTIF";
    case OP_VERIF                   : return "OP_VERIF";
    case OP_VERNOTIF                : return "OP_VERNOTIF";
    case OP_ELSE                    : return "OP_ELSE";
    case OP_ENDIF                   : return "OP_ENDIF";
    case OP_VERIFY                  : return "OP_VERIFY";
    case OP_RETURN                  : return "OP_RETURN";

    // stack ops    
    case OP_TOALTSTACK              : return "OP_TOALTSTACK";
    case OP_FROMALTSTACK            : return "OP_FROMALTSTACK";
    case OP_2DROP                   : return "OP_2DROP";
    case OP_2DUP                    : return "OP_2DUP";
    case OP_3DUP                    : return "OP_3DUP";
    case OP_2OVER                   : return "OP_2OVER";
    case OP_2ROT                    : return "OP_2ROT";
    case OP_2SWAP                   : return "OP_2SWAP";
    case OP_IFDUP                   : return "OP_IFDUP";
    case OP_DEPTH                   : return "OP_DEPTH";
    case OP_DROP                    : return "OP_DROP";
    case OP_DUP                     : return "OP_DUP";
    case OP_NIP                     : return "OP_NIP";
    case OP_OVER                    : return "OP_OVER";
    case OP_PICK                    : return "OP_PICK";
    case OP_ROLL                    : return "OP_ROLL";
    case OP_ROT                     : return "OP_ROT";
    case OP_SWAP                    : return "OP_SWAP";
    case OP_TUCK                    : return "OP_TUCK";

    // splice ops   
    case OP_CAT                     : return "OP_CAT";
    case OP_SUBSTR                  : return "OP_SUBSTR";
    case OP_LEFT                    : return "OP_LEFT";
    case OP_RIGHT                   : return "OP_RIGHT";
    case OP_SIZE                    : return "OP_SIZE";

    // bit logic    
    case OP_INVERT                  : return "OP_INVERT";
    case OP_AND                     : return "OP_AND";
    case OP_OR                      : return "OP_OR";
    case OP_XOR                     : return "OP_XOR";
    case OP_EQUAL                   : return "OP_EQUAL";
    case OP_EQUALVERIFY             : return "OP_EQUALVERIFY";
    case OP_RESERVED1               : return "OP_RESERVED1";
    case OP_RESERVED2               : return "OP_RESERVED2";

    // numeric  
    case OP_1ADD                    : return "OP_1ADD";
    case OP_1SUB                    : return "OP_1SUB";
    case OP_2MUL                    : return "OP_2MUL";
    case OP_2DIV                    : return "OP_2DIV";
    case OP_NEGATE                  : return "OP_NEGATE";
    case OP_ABS                     : return "OP_ABS";
    case OP_NOT                     : return "OP_NOT";
    case OP_0NOTEQUAL               : return "OP_0NOTEQUAL";
    case OP_ADD                     : return "OP_ADD";
    case OP_SUB                     : return "OP_SUB";
    case OP_MUL                     : return "OP_MUL";
    case OP_DIV                     : return "OP_DIV";
    case OP_MOD                     : return "OP_MOD";
    case OP_LSHIFT                  : return "OP_LSHIFT";
    case OP_RSHIFT                  : return "OP_RSHIFT";
    case OP_BOOLAND                 : return "OP_BOOLAND";
    case OP_BOOLOR                  : return "OP_BOOLOR";
    case OP_NUMEQUAL                : return "OP_NUMEQUAL";
    case OP_NUMEQUALVERIFY          : return "OP_NUMEQUALVERIFY";
    case OP_NUMNOTEQUAL             : return "OP_NUMNOTEQUAL";
    case OP_LESSTHAN                : return "OP_LESSTHAN";
    case OP_GREATERTHAN             : return "OP_GREATERTHAN";
    case OP_LESSTHANOREQUAL         : return "OP_LESSTHANOREQUAL";
    case OP_GREATERTHANOREQUAL      : return "OP_GREATERTHANOREQUAL";
    case OP_MIN                     : return "OP_MIN";
    case OP_MAX                     : return "OP_MAX";
    case OP_WITHIN                  : return "OP_WITHIN";

    // crypto   
    case OP_RIPEMD160               : return "OP_RIPEMD160";
    case OP_SHA1                    : return "OP_SHA1";
    case OP_SHA256                  : return "OP_SHA256";
    case OP_HASH160                 : return "OP_HASH160";
    case OP_HASH256                 : return "OP_HASH256";
    case OP_CODESEPARATOR           : return "OP_CODESEPARATOR";
    case OP_CHECKSIG                : return "OP_CHECKSIG";
    case OP_CHECKSIGVERIFY          : return "OP_CHECKSIGVERIFY";
    case OP_CHECKMULTISIG           : return "OP_CHECKMULTISIG";
    case OP_CHECKMULTISIGVERIFY     : return "OP_CHECKMULTISIGVERIFY";

    // expansion    
    case OP_NOP1                    : return "OP_NOP1";
    case OP_CHECKLOCKTIMEVERIFY     : return "OP_CHECKLOCKTIMEVERIFY";
    case OP_CHECKSEQUENCEVERIFY     : return "OP_CHECKSEQUENCEVERIFY";
    case OP_NOP4                    : return "OP_NOP4";
    case OP_NOP5                    : return "OP_NOP5";
    case OP_NOP6                    : return "OP_NOP6";
    case OP_NOP7                    : return "OP_NOP7";
    case OP_NOP8                    : return "OP_NOP8";
    case OP_NOP9                    : return "OP_NOP9";
    case OP_NOP10                   : return "OP_NOP10";

    case OP_INVALIDOPCODE           : return "OP_INVALIDOPCODE";

    default                         : return "OP_UNKNOWN";
    }
}

std::string Address::print(int n)
{
    std::string string;
    string =  dent(n) + "Address{\n";
    string += dent(n+1) + "addressFlag = " + std::to_string(addressFlag) + "\n";
    string += dent(n+1) + "raw = " + raw + "\n";
    string += dent(n+1) + "raw_size = " + std::to_string(raw.size()) + "\n";
    string += dent(n+1) + "address = " + address + "\n";
    string += dent(n+1) + "type = " + GetAddressDecoderTypeName(type) + "\n";
    if(decoded.size())
    {
        string += dent(n+1) + "decoded = " + "\n";
        string += dent(n+1);
        for(std::vector<std::string>::iterator it = decoded.begin(); it != decoded.end(); it++)
        {
            if((*it).size() == sizeof(uint8_t))
                string += GetOpName(OpCodeType(*(uint8_t*)(*it).c_str()));
            else
                string += *it + " ";
            string += " ";
        }
        string += "\n";
    }
    string += dent(n) + "}\n";
    return string;
}

bool Address::IsValid(){return ((addressFlag == true) && (type != FAIL));}

std::string sha256(const std::string& data)
{
    hashwrapper* sha256 = new sha256wrapper();
    std::string hash_sha256 (sha256->getHashFromString(data));
    delete sha256;
    return hash_sha256;
}

std::string doublesha256(const std::string& data){return sha256(sha256(data));}

std::string ripemd(const std::string& data)
{
    uint160 hash_buffer;
    ripemd160((const unsigned char*)data.c_str(), data.size(), hash_buffer);
    std::string hash_ripemd160((char *)hash_buffer, 20);
    return hash_ripemd160;
}

std::string ripemdsha(std::string& data){return ripemd(sha256(data));}

bool AddressDecoder::PK_check(std::string script)
{
    secp256k1_pubkey pubkey;
    if(!secp256k1_ec_pubkey_parse(ctx, &pubkey, (const unsigned char *)script.c_str(), script.size()))
        return false;
    return true;
}

bool AddressDecoder::PK_decompress(std::string script, std::string& pub_key)
{
    script[0] = ((char)script.c_str()[0]) - 2;
    secp256k1_pubkey pubkey;
    if(!secp256k1_ec_pubkey_parse(ctx, &pubkey, (const unsigned char *)script.c_str(), 33))
        return false;
    unsigned char pub[65];
    size_t len = sizeof(pub);
    if(!secp256k1_ec_pubkey_serialize(ctx, pub, &len, &pubkey, SECP256K1_EC_UNCOMPRESSED))
        return false;
    pub_key = std::string((char*)pub, 65);
    return true;
}

std::string AddressDecoder::base58(const std::string& data, unsigned char id)
{
    std::string insert;
    insert.reserve(data.size() + 1);
    insert = (unsigned char)id;
    insert += data;
    std::string checsum = doublesha256(insert).substr(0,4);
    insert += checsum;
    return EncodeBase58(insert);
}

std::string AddressDecoder::bech32(const std::string& script)
{
    data values (script.begin(), script.end());
    return EncodeBech32(Encoding::BECH32, "bc", values);
}

std::string AddressDecoder::base58_P2PKH(std::string& script){return base58(script, 0);}

std::string AddressDecoder::base58_P2SH(std::string& script){return base58(script, 5);}

std::string AddressDecoder::base58_P2PK_ripemdsha(std::string& script){return base58(ripemdsha(script), 0);}

std::string AddressDecoder::base58_P2S_ripemdsha(std::string& script){return base58(ripemdsha(script), 5);}

std::string AddressDecoder::bech32_P2W(std::string& script){return bech32(script);}

void AddressDecoder::scriptPubKey_parse(Address* addr_buf)
{
    BStream bstream (&addr_buf->raw);
    uint64_t length = bstream.getLength();
    while(bstream.getPos() < length)
    {
        uint8_t curr_byte = bstream.read<uint8_t>();
        
        addr_buf->decoded.push_back(std::string((const char*)&curr_byte, sizeof(uint8_t)));

        if(curr_byte >= 0x01 && curr_byte <= 0x4b)
        {
            addr_buf->decoded.pop_back();
            if(bstream.getPos() + curr_byte > length)
            {
                addr_buf->type = FAIL;
                break;
            }
            addr_buf->decoded.push_back(bstream.read(curr_byte));
        }
        else if(curr_byte == OP_PUSHDATA1)
            addr_buf->decoded.push_back(bstream.read(1));
        else if(curr_byte == OP_PUSHDATA2)
            addr_buf->decoded.push_back(bstream.read(2));
        else if(curr_byte == OP_PUSHDATA4)
            addr_buf->decoded.push_back(bstream.read(4));
        else if(curr_byte == OP_RETURN || GetOpName(OpCodeType(curr_byte)) == "OP_UNKNOWN")
        {
            addr_buf->type = FAIL;
            break;
        }
    }
}

void AddressDecoder::addressDecode(Address* addr_buf, BStream* bstream, const AddressDecoderMode& mode)
{
    addr_buf->type = NONE;
    addr_buf->addressFlag = false;

    if(mode == TYPE)
    {   
        uint64_t scriptLength = bstream->readVarInt();

        if(scriptLength < SPECIAL_SCRIPT_SIZE)
        {
            // base58 P2PKH
            if(scriptLength == 0x00)
            {
                addr_buf->type = P2PKH;
                addr_buf->raw = bstream->read(20);
                addr_buf->address = base58_P2PKH(addr_buf->raw);
                addr_buf->addressFlag = true;
            }
            // base58 P2SH
            else if(scriptLength == 0x01)
            {
                addr_buf->type = P2SH;
                addr_buf->raw = bstream->read(20);
                addr_buf->address = base58_P2SH(addr_buf->raw);
                addr_buf->addressFlag = true;
            }
            // base58 P2PKH no ripemdsha
            else if(scriptLength == 0x02 || scriptLength == 0x03)
            {
                addr_buf->type = P2PKH_NO_RIPEMDSHA;
                addr_buf->raw = bstream->read(32);

                std::string insert;
                insert += (char)scriptLength;
                insert += addr_buf->raw;

                if(PK_check(insert))
                {
                    addr_buf->address = base58_P2PK_ripemdsha(insert);
                    addr_buf->addressFlag = true;
                }
            }
            // base58 P2PKH compressed
            else if(scriptLength == 0x04 || scriptLength == 0x05)
            {
                addr_buf->type = P2PKH_COMPRESSED;
                addr_buf->raw = bstream->read(32);

                std::string insert;
                insert += (char)scriptLength;
                insert += addr_buf->raw;

                std::string pub_key;
                if(PK_decompress(insert, pub_key))
                {
                    addr_buf->address = base58_P2PK_ripemdsha(pub_key);
                    addr_buf->addressFlag = true;
                }
            }
        }
        else
        {
            scriptLength -= SPECIAL_SCRIPT_SIZE;
            addr_buf->raw = bstream->read(scriptLength);
            scriptPubKey_parse(addr_buf);
            if(addr_buf->type == FAIL)
                return;

            if(addr_buf->decoded.size() == 2)
            { 
                // burn
                if((addr_buf->decoded[0].size() == sizeof(uint8_t) && *(uint8_t*)(addr_buf->decoded[0]).c_str() == OP_RETURN) || (addr_buf->decoded[1].size() == sizeof(uint8_t) && *(uint8_t*)addr_buf->decoded[1].c_str() == OP_RETURN))
                {
                    addr_buf->type = FAIL;
                    addr_buf->addressFlag = true;
                }
                // bech32_P2WPKH
                else if(*(uint8_t*)addr_buf->decoded[0].c_str() == OP_0 && addr_buf->decoded[0].size() == sizeof(uint8_t) && addr_buf->decoded[1].size() == 20)
                {
                    addr_buf->type = P2WPKH;
                    addr_buf->address = bech32_P2W(addr_buf->decoded[1]);
                    addr_buf->addressFlag = true;
                }
                // bech32_P2WSH
                else if(*(uint8_t*)addr_buf->decoded[0].c_str() == OP_0 && addr_buf->decoded[0].size() == sizeof(uint8_t) && addr_buf->decoded[1].size() == 32)
                {
                    addr_buf->type = P2WSH;
                    addr_buf->address = bech32_P2W(addr_buf->decoded[1]);
                    addr_buf->addressFlag = true;
                }
            }
            else
            {
                addr_buf->type = OTHER;
                addr_buf->address = addr_buf->raw;
                addr_buf->addressFlag = true;
            }
        }
    }
    else if(mode == SCRIPT)
    {
        uint64_t scriptLength = bstream->readCompactSize();
        addr_buf->raw = bstream->read(scriptLength);
        scriptPubKey_parse(addr_buf);
        if(addr_buf->type == FAIL)
            return;

        if(addr_buf->decoded.size() == 2)
        {
            // burn
            if((addr_buf->decoded[0].size() == sizeof(uint8_t) && *(uint8_t*)(addr_buf->decoded[0]).c_str() == OP_RETURN) || (addr_buf->decoded[1].size() == sizeof(uint8_t) && *(uint8_t*)addr_buf->decoded[1].c_str() == OP_RETURN))
            {
                addr_buf->type = FAIL;
                addr_buf->addressFlag = true;
            }
            // bech32_P2WPKH
            else if(*(uint8_t*)addr_buf->decoded[0].c_str() == OP_0 && addr_buf->decoded[0].size() == sizeof(uint8_t) && addr_buf->decoded[1].size() == 20)
            {
                addr_buf->type = P2WPKH;
                addr_buf->address = bech32_P2W(addr_buf->decoded[1]);
                addr_buf->addressFlag = true;
            }
            // bech32_P2WSH
            else if(*(uint8_t*)addr_buf->decoded[0].c_str() == OP_0 && addr_buf->decoded[0].size() == sizeof(uint8_t) && addr_buf->decoded[1].size() == 32)
            {
                addr_buf->type = P2WSH;
                addr_buf->address = bech32_P2W(addr_buf->decoded[1]);
                addr_buf->addressFlag = true;
            }
            // base58_P2PKH
            else if(*(uint8_t*)addr_buf->decoded[1].c_str() == OP_CHECKSIG && addr_buf->decoded[1].size() == sizeof(uint8_t))
            {
                if(addr_buf->decoded[0].size() == 65)
                {
                    addr_buf->type = P2PKH_NO_RIPEMDSHA;
                    if(PK_check(addr_buf->decoded[0]))
                    {
                        addr_buf->address = base58_P2PK_ripemdsha(addr_buf->decoded[0]);
                        addr_buf->addressFlag = true;
                    }
                }
                else if(addr_buf->decoded[0].size() == 33)
                {
                    std::string pub_key;
                    if(PK_decompress(addr_buf->decoded[0], pub_key))
                    {
                        addr_buf->type = P2PKH_COMPRESSED;
                        addr_buf->address = base58_P2PK_ripemdsha(pub_key);
                        addr_buf->addressFlag = true;
                    }
                }
            }
        }
        // base58_P2SH
        else if(*(uint8_t*)addr_buf->decoded[0].c_str() == OP_HASH160 && addr_buf->decoded[0].size() == sizeof(uint8_t) && addr_buf->decoded[1].size() == 20 && *(uint8_t*)addr_buf->decoded[2].c_str() == OP_EQUAL && addr_buf->decoded[2].size() == sizeof(uint8_t) && addr_buf->decoded.size() == 3)
        {
            addr_buf->type = P2SH;
            addr_buf->address = base58_P2SH(addr_buf->decoded[1]);
            addr_buf->addressFlag = true;
        }
        // base58_P2PKH
        else if(*(uint8_t*)addr_buf->decoded[0].c_str() == OP_DUP && addr_buf->decoded[0].size() == sizeof(uint8_t) && *(uint8_t*)addr_buf->decoded[1].c_str() == OP_HASH160 && addr_buf->decoded[1].size() == sizeof(uint8_t) && addr_buf->decoded[2].size() == 20 && *(uint8_t*)addr_buf->decoded[3].c_str() == OP_EQUALVERIFY && addr_buf->decoded[3].size() == sizeof(uint8_t) && *(uint8_t*)addr_buf->decoded[4].c_str() == OP_CHECKSIG && addr_buf->decoded[4].size() == sizeof(uint8_t) && addr_buf->decoded.size() == 5)
        {
            addr_buf->type = P2PKH;
            addr_buf->address = base58_P2PKH(addr_buf->decoded[2]);
            addr_buf->addressFlag = true;
        }
        // other (mostly P2MS)
        else
        {
            addr_buf->type = OTHER;
            addr_buf->address = addr_buf->raw;
            addr_buf->addressFlag = true;
        }
    }
}