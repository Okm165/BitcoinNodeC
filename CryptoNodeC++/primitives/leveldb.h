/*
LevelDb functionalty --> level database handler
------------------------------------------------------------------------------------------
In order to use LevelDb create object LevelDb
------------------------------------------------------------------------------------------
LevelDb(const char* path, bool create_if_missing)
level database handler serves level database read/write functionalities,
<path> is path to database,
<create_if_missing> flag that if true the leveldb will be created,
------------------------------------------------------------------------------------------
void update(std::string &key, std::string &val)
write key and val pair to db,
if key exists final value = prev_val + val,
if final value is <= 0 the key is erased or not written
------------------------------------------------------------------------------------------
void put(std::string &key, std::string &val)
write key and val pair to db regardless
------------------------------------------------------------------------------------------
std::string get(std::string &key)
read value with key <key>
------------------------------------------------------------------------------------------
uint64_t getLength(uint64_t length = 0)
get length of database (number of rows/pairs)
------------------------------------------------------------------------------------------
void saveDict(std::string path, bool create_if_missing, adDict& dict, bool progress)
void saveDict(std::string path, bool create_if_missing, amDict& dict, bool progress)
save adDict, amDict to database using update method
------------------------------------------------------------------------------------------
void compareDbs(std::string& path_1, std::string& path_2, bool progress)
compare two level databases and return match_counter and missmatch_counter
------------------------------------------------------------------------------------------
*/

#ifndef LEVELDB_H
#define LEVELDB_H

#include "util.h"
#include "os.h"
#include "leveldb/db.h"
#include "progressbar.h"


class LevelDb
{
    protected:
    leveldb::Options options;
    const char* path;
    
    public:
    leveldb::DB* db;
    leveldb::Iterator* it;
    

    LevelDb(){};
    LevelDb(const char* path, bool create_if_missing=false);
    
    void update(std::string &key, std::string &val);

    void put(std::string &key, std::string &val);

    std::string get(std::string &key);

    uint64_t getLength(uint64_t length = 0);
};

void saveDict(std::string path, bool create_if_missing, adDict& dict, bool progress = false);
void saveDict(std::string path, bool create_if_missing, amDict& dict, bool progress = false);
void compareDbs(std::string& path_1, std::string& path_2, bool progress = false);

#endif