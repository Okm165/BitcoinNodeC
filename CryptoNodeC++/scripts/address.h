#ifndef ADDRESS_H
#define ADDRESS_H

#include "../primitives/primitives.h"
#include "../crypto/crypto.h"

class OpCodes
{
    public:
    Json::Value json;
    const char* path;
    OpDict dict;
    secp256k1_context* ctx = NULL;

    OpCodes(){};
    OpCodes(const char* path);

    void createOpDict();
    
    std::vector<std::string> inScriptDecode(std::string& data);

    std::vector<std::string> outScriptDecode(std::string& data);

    std::string outScriptAddress(std::vector<std::string>& data);

    ~OpCodes()
    {
        secp256k1_context_destroy(ctx);
    };
};

std::string sha256(const std::string& data);

std::string doublesha256(const std::string& data);

std::string ripemd(const std::string& data);

std::string ripemdsha(std::string& data);

std::string decompressPK(std::string script, OpCodes& opcodes);

std::string base58(const std::string& data, unsigned char id);

std::string bech32(const std::string& script);

std::string base58_P2PKH(std::string& script);

std::string base58_P2SH(std::string& script);

std::string base58_P2PK_ripemdsha(std::string& script);

std::string bech32_P2W(std::string& script);

std::vector<std::string> scriptDecode(std::map<unsigned char, std::string>& dict, std::string& data);

std::string addressDecodeScript(std::vector<std::string>& data, OpCodes& opcodes);

std::string addressDecodeType(BStream& bstream, uint64_t& scriptType, OpCodes& opcodes);

#endif