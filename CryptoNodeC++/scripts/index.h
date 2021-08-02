/*
Index decode functionality --> decode index level database
------------------------------------------------------------------------------------------
index database consists info about blocks that helps to find them in rev/blk files quickly
------------------------------------------------------------------------------------------
IBlock readIBlock(Index* index, std::string& hash)
finds block by hash and returns useful data in IBlock object
------------------------------------------------------------------------------------------
Index(const char* path, bool create_if_missing=false) : LevelDb::LevelDb(path, create_if_missing)
creates index object needed by readIBlock
<path> is path to index database folder
------------------------------------------------------------------------------------------
*/

#ifndef INDEX_H
#define INDEX_H

#include "../primitives/primitives.h"

struct IBlock
{
    int64_t nVersion;                               // version
    uint64_t nHeight;                               // block height in blockchain
    uint64_t nStatus;                               // block status
    uint64_t nTx;                                   // number of transactions in block
    uint64_t nFile;                                 // file number in which the block is stored blk?????.dat/ rev?????.dat
    uint64_t nDataPos;                              // pointer in blk file where block is stored
    uint64_t nUndoPos;                              // pointer in rev file where undoblock is stored

    int32_t version;                                // block version header
    std::string hashPrev;                           // previous block hash
    std::string hashMerkleRoot;                     // merkle tree root hash
    uint32_t nTime;                                 // timestamp
    uint32_t nBits;                                 // nBits
    u_int32_t nNonce;                               // nNonce

    std::string print(int n = 0);
};

class Index : public LevelDb
{   
    public:
    Index(const char* path, bool create_if_missing=false) : LevelDb::LevelDb(path, create_if_missing){}
};

IBlock readIBlock(Index* index, std::string& hash);

#endif