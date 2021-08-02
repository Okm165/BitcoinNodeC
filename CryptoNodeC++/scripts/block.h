/*
Block read functionality --> read ?????blk.dat file
------------------------------------------------------------------------------------------
BBlock readBBlock(Index* index, AddressDecoder* addrdec, const char* blk_path, std::string& hash)
decodes block with hash <hash> and saves it to BBlock object
requires:   Index object pointer, 
            AddressDecoder object pointer,
            blk_path - path to blk file(s)
            hash of the block to decode
------------------------------------------------------------------------------------------
*/

#ifndef BLOCK_H
#define BLOCK_H

#include "index.h"
#include "address.h"

struct BTxIn
{
    uint64_t nHeight;                               // height in transaction input vector           
    Address address;                                // address object
    uint32_t sequence;                              // sequence value (not crucial)    
    std::vector<std::string> witnessScriptVec;      // witness script vector

    std::string print(int n = 0);
};

struct BTxOut
{
    uint64_t nHeight;                               // height in transaction output vector           
    uint64_t amount;                                // transaction output value in SAT (1 SAT = 1/100000000 BTC)           
    Address address;                                // address object

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
    uint32_t id;                                    // block id, the same for all blocks, helpful in debugging (magicID = 0xD9B4BEF9)
    uint32_t headerLength;                          // length of this block in bytes
    int32_t version;                                // block version header expected to be equal to 1 (0x00000001)
    std::string hashMerkleRoot;                     // merkle tree root hash
    uint32_t nTime;                                 // timestamp
    uint32_t nBits;                                 // nBits
    uint32_t nNonce;                                // nNonce
    std::vector<BTx> txVec;                         // vector of Tx objects (transaction objects / transaction info)

    std::string print(int n = 0);
};

std::vector<BTxIn> readInVector(BStream* bstream, AddressDecoder* addrdec);

std::vector<BTxOut> readOutVector(BStream* bstream, AddressDecoder* addrdec);

BBlock readBBlock(Index* index, AddressDecoder* addrdec, const char* blk_path, std::string& hash);

#endif