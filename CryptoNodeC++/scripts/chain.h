#ifndef CHAIN_H
#define CHAIN_H

#include "address.h"

struct CBlock
{
    std::string hash;                               // transaction hash
    uint64_t blkHeight;                             // block height the transaction is in
    uint64_t nHeight;                               // transfer height in transaction
    bool isCoinBase;                                // coinbase flag
    uint64_t amount;                                // transaction input value in SAT (1 SAT = 1/100000000 BTC)
    uint64_t scriptType;                            // transaction script type
    std::string address;                            // address

    std::string print(int n = 0);
};

class Chain : public LevelDb
{   
    public:

    uint64_t cursor;
    leveldb::Iterator* it;
    std::string obfuscation_key;                    // key to decode chain database
    std::string hash;                               // hash to which chain database holds
    
    Chain(const char* path, uint64_t cursor=0, bool create_if_missing=false);

    void getObfuscationKey();

    void getBlockHash();

    std::string applyObfuscationKey(const std::string& data);

    Row getRow();
};

CBlock readCBlock(Chain& chain, OpCodes& opcodes);

#endif