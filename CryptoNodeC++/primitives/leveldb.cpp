#include "leveldb.h"

LevelDb::LevelDb(const char* path, bool create_if_missing)
{
    this->path = path;
    options.create_if_missing = create_if_missing;
    // options.write_buffer_size = 4 * 1024 * 1024;
    // options.block_size = 1024 * 1024;
    // options.block_restart_interval = 1024;
    // options.max_open_files = 500;
    // options.max_file_size = 10 * 1024 * 1024;
    // options.block_cache = leveldb::NewLRUCache(100 * 1048576);  // 100MB cache
    // options.compression = leveldb::kNoCompression;
    leveldb::Status status = leveldb::DB::Open(options, path, &db);
    assert(status.ok());
}

void LevelDb::update(std::string &key, std::string &val)
{
    leveldb::Status status;
    
    leveldb::Slice db_key(key);
    leveldb::Slice db_val(val);

    int64_t val_int = *(int64_t *)val.data();
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
        int64_t new_value = val_int + *(int64_t *)db_val_ret.data();
        if(new_value > 0)
        {
            char* new_val = (char*)&new_value;
            leveldb::Slice db_new_val(new_val, sizeof(int64_t));
            status = db->Put(leveldb::WriteOptions(), db_key, db_new_val);
            assert(status.ok());
        }
        else
        {
            status = db->Delete(leveldb::WriteOptions(), db_key);
            assert(status.ok());
        }
    }
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

uint64_t LevelDb::getLength(uint64_t length)
{
    if(length){return length;}
    // get current position
    uint64_t counter = 0;
    it = db->NewIterator(leveldb::ReadOptions());
    it->SeekToFirst();
    while(it->Valid())
    {
        it->Next();
        counter++;
    }
    it->SeekToFirst();
    return counter;
}

void saveDict(std::string path, bool create_if_missing, adDict& dict, bool progress)
{
    if(dirExists(path.c_str())){clearDir(path);}
    LevelDb db(path.c_str(), create_if_missing);
    ProgressBar* bar;

    if(progress){bar = new ProgressBar("saving address dict...", dict.size(), PROGRESS_BAR_SETTINGS);}
    for(adDict::iterator it = dict.begin(); it != dict.end(); it++)
    {
        std::string key (it->first);
        std::string value ((const char*)&it->second, sizeof(uint64_t));
        db.update(key, value);
        if(progress){bar->update();}
    }
    if(progress){bar->close();}
    if(progress){delete bar;}
}

void saveDict(std::string path, bool create_if_missing, amDict& dict, bool progress)
{
    if(dirExists(path.c_str())){clearDir(path);}
    LevelDb db(path.c_str(), create_if_missing);
    ProgressBar* bar;

    if(progress){bar = new ProgressBar("saving amount dict...", dict.size(), PROGRESS_BAR_SETTINGS);}
    for(amDict::iterator it = dict.begin(); it != dict.end(); it++)
    {
        std::string key ((const char*)&it->first, sizeof(uint64_t));
        std::string value ((const char*)&it->second, sizeof(uint64_t));
        db.update(key, value);
        if(progress){bar->update();}
    }
    if(progress){bar->close();}
    if(progress){delete bar;}
}

void compareDbs(std::string& path_1, std::string& path_2, bool progress)
{
    LevelDb db_1 (path_1.c_str(), false);
    LevelDb db_2 (path_2.c_str(), false);

    if(progress){std::cout << "\r" << "calculating db_1 length..." << std::flush;}
    uint64_t db_1_length = db_1.getLength();
    if(progress){std::cout << "\r" << "chainstate length = " << db_1_length << "              " << std::endl;}
    if(progress){std::cout << "\r" << "calculating db_2 length..." << std::flush;}
    uint64_t db_2_length = db_2.getLength();
    if(progress){std::cout << "\r" << "chainstate length = " << db_2_length << "              " << std::endl;}
    
    if(db_1_length == db_2_length)
        std::cout << "\r" << "length matches" << std::endl;
    else
        std::cout << "\r" << "length missmatches --------!" << std::endl;
    uint64_t match_counter = 0;
    uint64_t missmatch_counter = 0;

    ProgressBar* bar;
    if(progress){bar = new ProgressBar ("checking...", db_1_length, PROGRESS_BAR_SETTINGS);}
    while(db_1.it->Valid())
    {
        leveldb::Slice db_1_key = db_1.it->key();
        leveldb::Slice db_1_value = db_1.it->value();
        std::string db_2_key = db_1_key.ToString();
        
        leveldb::Status status;
        std::string db_val_ret;
        status = db_2.db->Get(leveldb::ReadOptions(), db_1_key, &db_val_ret);
        
        if(status.IsNotFound())
        {
            missmatch_counter++;
        }
        else
        {
            if(*(uint64_t*)db_1_value.ToString().c_str() == *(uint64_t*)db_val_ret.c_str())
                match_counter++;
            else
                missmatch_counter++;
        }
        db_1.it->Next();
        if(progress){bar->update();}
    }
    if(progress){bar->close();}
    if(progress){delete bar;}

    std::cout << "match_counter = " << match_counter << std::endl;
    std::cout << "missmatch_counter = " << missmatch_counter << std::endl;
}