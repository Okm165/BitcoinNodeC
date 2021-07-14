#ifndef BLOCK_H
#define BLOCK_H

#include "index.h"
#include "address.h"

struct BTxIn
{
    uint64_t nHeight;                               // height in transaction input vector           
    uint64_t scriptLength;                          // length of SCRIPT programme
    std::string script;                             // TxIn script code
    uint32_t sequence;                              // sequence value (not crucial)    
    std::vector<std::string> witnessScriptVec;      // witness script vector

    std::string print(int n = 0);
};

struct BTxOut
{
    uint64_t nHeight;                               // height in transaction output vector           
    uint64_t amount;                                // transaction output value in SAT (1 SAT = 1/100000000 BTC)           
    uint64_t scriptLength;                          // length of SCRIPT programme
    std::string script;                             // TxOut script code          
    std::vector<std::string> scriptDecoded;         // decoded script programme
    std::string address;                            // decoded script address

    std::string print(int n = 0);
};

struct BTx
{
    std::string hash;                               // hash of transaction
    int64_t version;                                // transaction version
    uint64_t nHeight;                               // transaction height in block
    std::vector<BTxIn> inVec;                       // vector of TxIn objects
    std::vector<BTxOut> outVec;                     // vector of TxOut objects
    uint64_t nLockTime;                             // transaction lock time

    std::string print(int n = 0);
};

struct BBlock
{
    std::string hash;                               // hash of block
    std::string hashPrev;                           // previous block hash
    uint64_t nHeight;                               // block height in blockchain
    uint32_t id;                                    // block id, the same for all blocks , helpful in debugging
    uint32_t headerLength;                          // length of this block in bytes
    int32_t version;                                // block version header expected to be equal to 1 (0x00000001)
    std::string hashMerkleRoot;                     // merkle tree root hash
    uint32_t nTime;                                 // timestamp
    uint32_t nBits;                                 // nBits
    uint32_t nNonce;                                // nNonce
    std::vector<BTx> txVec;                         // vector of Tx objects (transaction objects / transaction info)

    std::string print(int n = 0);
};

std::vector<BTxIn> readInVector(BStream& bstream, OpCodes& opcodes);

std::vector<BTxOut> readOutVector(BStream& bstream, OpCodes& opcodes);

BBlock readBBlock(Index& index, OpCodes& opcodes, const char * blk_path, std::string& hash);

#endif