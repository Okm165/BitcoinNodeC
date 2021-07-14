#include "address.h"

std::string sha256(const std::string& data)
{
    hashwrapper* sha256 = new sha256wrapper();
    std::string hash_sha256 (sha256->getHashFromString(data));
    delete sha256;
    return hash_sha256;
}

std::string doublesha256(const std::string& data){return sha256(sha256(data));}

std::string ripemd160(const std::string& data)
{
    uint160 hash_buffer;
    ripemd160((const unsigned char*)data.c_str(), data.size(), hash_buffer);
    std::string hash_ripemd160((char *)hash_buffer, 20);
    return hash_ripemd160;
}

std::string ripemdsha(std::string& data){return ripemd160(sha256(data));}

std::string decompressPK(std::string script, OpCodes& opcodes)
{
    script[0] = ((char)script.c_str()[0]) - 2;
    secp256k1_pubkey pubkey;

    assert(secp256k1_ec_pubkey_parse(opcodes.ctx, &pubkey, (const unsigned char *)HexToString(script).c_str(), 33));
    unsigned char pub[65];
    size_t len = sizeof(pub);
    assert(secp256k1_ec_pubkey_serialize(opcodes.ctx, pub, &len, &pubkey, SECP256K1_EC_UNCOMPRESSED));
    return std::string((char*)pub, 65);
}

std::string base58(const std::string& data, unsigned char id)
{
    std::string insert;
    insert.reserve(data.size() + 1);
    insert = (unsigned char)id;
    insert += data;
    std::string checsum = doublesha256(insert).substr(0,4);
    insert += checsum;
    return EncodeBase58(insert);
}

std::string bech32(const std::string& script)
{
    data values (script.begin(), script.end());
    return EncodeBech32(Encoding::BECH32, "bc", values);
}

std::string base58_P2PKH(std::string& script){return base58(script, 0);}

std::string base58_P2SH(std::string& script){return base58(script, 5);}

std::string base58_P2PK_ripemdsha(std::string& script){return base58(ripemdsha(script), 0);}

std::string bech32_P2W(std::string& script){return bech32(script);}

std::vector<std::string> scriptDecode(std::map<unsigned char, std::string>& dict, std::string& data)
{
    std::vector<std::string> decoded;
    BStream stream(data);
    uint32_t length = data.size();
    while (stream.getPos() < length)
    {
        unsigned char curr_byte = stream.read<char>();
        assert(dict[curr_byte].size());
        decoded.push_back(dict[curr_byte]);

        if(dict[curr_byte] == "N/A")
        {
            decoded.pop_back();
            decoded.push_back(stream.read((int)curr_byte));
        }
        else if(dict[curr_byte] == "OP_PUSHDATA1")
            decoded.push_back(stream.read(1));
        else if(dict[curr_byte] == "OP_PUSHDATA2")
            decoded.push_back(stream.read(2));
        else if(dict[curr_byte] == "OP_PUSHDATA4")
            decoded.push_back(stream.read(4));
        else if(dict[curr_byte] == "OP_RETURN")
            decoded.push_back(stream.readToEnd());
    }   
    return decoded;
}

std::string addressDecodeScript(std::vector<std::string>& data, OpCodes& opcodes)
{
    std::string address;
    bool success = false;
    
    if(data.size() == 2)
    {
        // burn
        if (data[0] == "OP_RETURN" || data[1] == "OP_RETURN")
        {
            address = "";
            success = true;
        }
        // bech32_P2WPKH
        if (data[0] == "OP_0" && data[1].size() == 20)
        {
            address = bech32_P2W(data[1]);
            success = true;
        }
        // bech32_P2WSH
        if (data[0] == "OP_0" && data[1].size() == 32)
        {
            address = bech32_P2W(data[1]);
            success = true;
        }
        // Bech32_P2PK
        if (data[1] == "OP_CHECKSIG")
        {
            if (data[0].size() == 65)
            {
                address = base58_P2PK_ripemdsha(data[0]);
                success = true;
            }
            if (data[0].size() == 33)
            {
                std::string pub_key = decompressPK(data[0], opcodes);
                address = base58_P2PK_ripemdsha(pub_key);
                success = true;
            }
        }
    }
    // base58_P2SH
    else if(data.size() == 3)
    {
        if (data[0] == "OP_HASH160" && data[1].size() == 20 && data[2] == "OP_EQUAL")
        {
            address = base58_P2SH(data[1]);
            success = true;
        }
    }
    // base58_P2PKH
    else if(data.size() == 5)
    {
        if (data[0] == "OP_DUP" && data[1] == "OP_HASH160" && data[2].size() == 20 && data[3] == "OP_EQUALVERIFY" && data[4] == "OP_CHECKSIG")
        {
            address = base58_P2PKH(data[2]);
            success = true;
        }
    }
    return address;
}

std::string addressDecodeType(BStream& bstream, uint64_t& scriptType, OpCodes& opcodes)
{
    std::string address;
    bool success = false;

    // base58 P2PKH
    if(scriptType == 0x00)
    {
        std::string script = bstream.read(20);
        address = base58_P2PKH(script);
        success = true;
    }
    // base58 P2SH
    else if(scriptType == 0x01)
    {
        std::string script = bstream.read(20);
        address = base58_P2SH(script);
        success = true;
    }
    // base58 P2PKH no ripemdsha
    else if(scriptType == 0x02 || scriptType == 0x03)
    {
        std::string script = bstream.read(32);
        std::string insert;
        insert += (char)scriptType;
        insert += script;
        address = base58_P2PK_ripemdsha(insert);
        success = true;
    }
    // base58 P2PKH compressed
    else if(scriptType == 0x04 || scriptType == 0x05)
    {
        std::string script = bstream.read(32);
        std::string insert;
        insert += (char)scriptType;
        insert += script;
        std::string pub_key = decompressPK(script, opcodes);
        address = base58_P2PK_ripemdsha(pub_key);
        success = true;
    }
    // bech32 P2WPKH
    else if(scriptType == 0x1c)
    {
        std::string script = bstream.read(22).substr(2);
        address = bech32_P2W(script);
        success = true;
    }
    // bech32 P2WSH
    else if(scriptType == 0x28)
    {
        std::string script = bstream.read(34).substr(2);
        address = bech32_P2W(script);
        success = true;
    }
    return address;
}

OpCodes::OpCodes(const char* path)
{
    this-> path = path;
    json = getJson(path);
    createOpDict();
}

void OpCodes::createOpDict()
{
    for (auto it = json.begin(); it != json.end(); ++it){
        if(it.name().size() > 4)
        {
            unsigned char start = *(unsigned char *)HexToString(it.name().substr(2,2)).c_str();
            unsigned char stop = *(unsigned char *)HexToString(it.name().substr(7,2)).c_str();
            while(stop >= start)
            {
                dict[start] = it->asString();
                start++;
            }
        }
        else
        {
            dict[*(unsigned char *)HexToString(it.name().substr(2,2)).c_str()] = it->asString();
        }
    }
}

std::vector<std::string> OpCodes::inScriptDecode(std::string& data){return scriptDecode(dict, data);}
std::vector<std::string> OpCodes::outScriptDecode(std::string& data){return scriptDecode(dict, data);}
std::string OpCodes::outScriptAddress(std::vector<std::string>& data){return addressDecodeScript(data, *this);}