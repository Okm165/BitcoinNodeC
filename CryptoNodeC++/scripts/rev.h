/*
UndoBlock read functionality --> read ?????rev.dat file
------------------------------------------------------------------------------------------
RBlock readRBlock(Index* index, AddressDecoder* opcodes, const char* rev_path, std::string& hash)
decodes block with hash <hash> and saves it to RBlock object
requires:   Index object pointer, 
            AddressDecoder object pointer,
            blk_path - path to blk file(s)
            hash of the block to decode
------------------------------------------------------------------------------------------
*/

#ifndef REV_H
#define REV_H

#include "index.h"
#include "address.h"

struct RTxIn
{
    uint64_t txHeight;                              // transaction height in block
    uint64_t nHeight;                               // transfer height in transaction
    bool isCoinBase;                                // coinbase flag
    int64_t version;                                // transaction version
    uint64_t amount;                                // transaction input value in SAT (1 SAT = 1/100000000 BTC)
    Address address;                                // address object

    std::string print(int n = 0);
};

struct RBlock
{
    std::string hash;                               // hash of block
    std::string hashPrev;                           // previous block hash
    uint64_t nHeight;                               // block height in blockchain
    uint32_t id;                                    // block id, the same for all blocks, helpful in debugging (magicID = 0xD9B4BEF9)
    uint32_t headerLength;                          // length of this block in bytes
    uint64_t nFile;                                 // file number in which the block is stored blk?????.dat/ rev?????.dat
    uint64_t nDataPos;                              // pointer in blk file where block is stored
    uint64_t nUndoPos;                              // pointer in rev file where undoblock is stored
    std::vector<RTxIn> inVec;                       // vector of TxIn objects
    std::string checksum;                           // checksum of entire UndoBlock for debug purposes

    std::string print(int n = 0);
};

RBlock readRBlock(Index* index, AddressDecoder* addrdec, const char* rev_path, std::string& hash);

#endif