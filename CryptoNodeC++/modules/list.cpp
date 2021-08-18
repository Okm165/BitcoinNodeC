#include "../scripts/scripts.h"

int main()
{
    Dict paths = composeJsonDict("../config/paths.json");
    std::string path = paths["cache"] + "0000000000000000000ded665b985a4000622a8529ea05cc0b395cefd894c659" + "/amountDb";
    // std::string path = paths["cache"] + "000000000000000000131c796dae0ffdb115279d3773a34483bf61ec41caeff7" + "/addressDb";
    LevelDb db (path.c_str());
    db.it = db.db->NewIterator(leveldb::ReadOptions());

    db.it->SeekToFirst();
    while(db.it->Valid())
    {
        std::string key = db.it->key().ToString();
        std::string value = db.it->value().ToString();
        uint64_t key_int = *(uint64_t*)key.c_str();
        uint64_t key_reversed_endian = switchEndian(key_int);

        std::cout << key_reversed_endian << " => " << *(uint64_t*)value.c_str() << std::endl;
        // std::cout << key << " => " << *(uint64_t*)value.c_str() << std::endl;

        db.it->Next();
    }
}