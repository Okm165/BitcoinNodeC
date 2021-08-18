/*
Address decode functionality --> decode scriptPubKey
------------------------------------------------------------------------------------------
In order to use address decoder create object AddressDecoder and call addressDecode() functionality.
------------------------------------------------------------------------------------------
void addressDecode(Address* addr_buf, BStream* bstream, const AddressDecoderMode& mode)
decodes address from <bstream> and saves it to <addr_buf>, in specified mode <mode>,
modes are: TYPE or SCRIPT, 
TYPE    -- chainstate_db and ?????rev.dat  
SCRIPT  -- ?????blk.dat
------------------------------------------------------------------------------------------
*/

#ifndef ADDRESS_H
#define ADDRESS_H

#include "opcodes.h"
#include "../crypto/crypto.h"
#include "../primitives/primitives.h"

enum AddressDecoderMode
{
    TYPE,                                       // type defined encoding
    SCRIPT,                                     // script defined encoding
};

const char* GetAddressDecoderModeName(const AddressDecoderMode& mode);

enum AddressDecoderType
{
    NONE,                                       // default value
    FAIL,                                       // failed to decode scriptPubKey or scriptPubKey results in failture
    BURN,                                       // burning coins transaction
    P2SH,                                       // pay to script hash
    P2PKH,                                      // pay to public key hash
    P2WSH,                                      // pay to witness script hash
    OTHER,                                      // not defined scriptPubKey
    P2WPKH,                                     // pay to witness public key hash
    P2PKH_COMPRESSED,                           // pay to compressed public key
    P2PKH_NO_RIPEMDSHA,                         // pay to unhashed public key
};

const char* GetAddressDecoderTypeName(const AddressDecoderType& type);

struct Address
{
    bool addressFlag;                           // true if address successfuly decoded, false otherwise 
    std::string address;                        // decoded address string
    AddressDecoderType type;                    // address type
    std::vector<std::string> decoded;           // entire decoded vector 

    std::string print(uint8_t n = 0);
    bool IsValid();
};

class AddressDecoder
{
    protected:
    secp256k1_context* ctx = NULL;

    public:
    AddressDecoder(){};
    ~AddressDecoder()
    {
        secp256k1_context_destroy(ctx);
    }

    void scriptPubKey_parse(Address* addr_buf, std::string* data);
    void addressDecode(Address* addr_buf, std::string* data, const AddressDecoderMode& mode);
    
    bool PK_check(std::string data);
    bool PK_decompress(std::string data, std::string& pub_key);
    std::string base58(const std::string& data, unsigned char id);
    std::string bech32(const std::string& data);
    std::string base58_P2PKH(const std::string& data);
    std::string base58_P2SH(const std::string& data);
    std::string base58_P2PK_ripemdsha(std::string& data);
    std::string base58_P2S_ripemdsha(std::string& data);
    std::string bech32_P2W(std::string& data);
};

std::string sha256(const std::string& data);
std::string doublesha256(const std::string& data);
std::string ripemd(const std::string& data);
std::string ripemdsha(std::string& data);

#endif