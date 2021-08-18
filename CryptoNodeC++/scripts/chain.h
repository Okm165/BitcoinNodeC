/*
Chainstate decode functionality --> decode chainstate level database
------------------------------------------------------------------------------------------
chainstate database consists unspent UTxOuts
------------------------------------------------------------------------------------------
CBlock readCBlock(Chain* chain, AddressDecoder* opcodes)
decodes address from chainstate row and saves it to CBlock object
it decodes only one row, repeat readCBlock() <getLength()> of times to get whole chainstate
------------------------------------------------------------------------------------------
Chain(const char* path, uint64_t cursor=0, bool create_if_missing=false)
creates chainstate object needed by readCBlock
<path> is path to chainstate database folder
------------------------------------------------------------------------------------------
uint64_t getLength(uint64_t length = 0)
return total length of chainstate level database, or if length is specified returns <length>
------------------------------------------------------------------------------------------
*/

#ifndef CHAIN_H
#define CHAIN_H

#include "address.h"

struct CTx
{
    std::string hash;                               // transaction hash
    uint64_t blkHeight;                             // block height the transaction is in
    uint64_t nHeight;                               // transfer height in transaction
    bool isCoinBase;                                // coinbase flag
    uint64_t amount;                                // transaction input value in SAT (1 SAT = 1/100000000 BTC)
    std::string scriptSig;                          // scriptSig

    std::string print(uint8_t n = 0);
};

class Chain : public LevelDb
{   
    public:

    leveldb::Iterator* cit;
    std::string obfuscation_key;                    // key to decode chain database
    std::string hash;                               // hash to which chain database holds
    
    Chain(){};
    Chain(const char* path, bool create_if_missing=false);

    void getObfuscationKey();

    void getBlockHash();

    uint64_t getLength(uint64_t length = 0);

    std::string applyObfuscationKey(const std::string& data);

    Row getRow();
};

CTx readCTx(Chain* chain);
std::string composeCTxHash(std::string& hash, uint32_t n);
CTx fetchCTx(Chain* chain, std::string& cTxHash);

#endif