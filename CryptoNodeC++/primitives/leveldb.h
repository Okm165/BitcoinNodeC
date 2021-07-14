#ifndef LEVELDB_H
#define LEVELDB_H

#include "util.h"
#include "leveldb/db.h"


class LevelDb
{
    public:
    leveldb::DB* db;
    const char* path;

    private:
    leveldb::Options options;
    
    public:

    LevelDb(const char* path, bool create_if_missing=false);

    void update(std::string &key, std::string &val);

    void put(std::string &key, std::string &val);

    std::string get(std::string &key);
};

#endif