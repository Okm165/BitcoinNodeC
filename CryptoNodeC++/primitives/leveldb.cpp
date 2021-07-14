#include "leveldb.h"

LevelDb::LevelDb(const char* path, bool create_if_missing)
{
    this->path = path;
    options.create_if_missing = create_if_missing;
    options.write_buffer_size = 32 * 1024 * 1024;
    options.max_open_files = 5000;
    options.max_file_size = 100 * 1024 * 1024;
    leveldb::Status status = leveldb::DB::Open(options, path, &db);
    assert(status.ok());
}

void LevelDb::update(std::string &key, std::string &val)
{
    leveldb::Status status;
    
    leveldb::Slice db_key(key);
    leveldb::Slice db_val(val);

    uint32_t val_int = *(unsigned int *)val.data();
    std::string db_val_ret;

    status = db->Get(leveldb::ReadOptions(), db_key, &db_val_ret);
    assert(!status.IsIOError());

    if(status.IsNotFound())
    {
        if(val_int > 0)
        {
            status = db->Put(leveldb::WriteOptions(), db_key, db_val);
            assert(status.ok());
        }
    }
    else
    {
        unsigned int new_value = val_int + *(unsigned int *)db_val_ret.data();
        char* new_val = (char*)&new_value;
        leveldb::Slice db_new_val(new_val, sizeof(int));
        if(new_value > 0)
        {
            status = db->Put(leveldb::WriteOptions(), db_key, db_new_val);
            assert(status.ok());
        }
        else
        {
            status = db->Delete(leveldb::WriteOptions(), db_key);
            assert(status.ok());
        }
    }
    status = db->Get(leveldb::ReadOptions(), db_key, &db_val_ret);
    assert(status.ok());
}
    
void LevelDb::put(std::string &key, std::string &val)
{
    leveldb::Status status;
    
    leveldb::Slice db_key(key);
    leveldb::Slice db_val(val);
    status = db->Put(leveldb::WriteOptions(), db_key, db_val);
    assert(status.ok());
}

std::string LevelDb::get(std::string &key)
{
    leveldb::Status status;
    std::string db_val_ret;
    leveldb::Slice db_key(key.c_str(), key.size());
    status = db->Get(leveldb::ReadOptions(), db_key, &db_val_ret);
    assert(status.ok());
    return db_val_ret;
}