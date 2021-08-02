#include "../scripts/scripts.h"

AddressDecoder addrdec;

void validateDbs(std::string& path_1, std::string& path_2)
{
    LevelDb db_1 (path_1.c_str(), false);
    LevelDb db_2 (path_2.c_str(), false);

    std::cout << "\r" << "calculating db_1 length..." << std::flush;
    uint64_t db_1_length = db_1.getLength();
    std::cout << "\r" << "chainstate length = " << db_1_length << "              " << std::endl;
    std::cout << "\r" << "calculating db_2 length..." << std::flush;
    uint64_t db_2_length = db_2.getLength();
    std::cout << "\r" << "chainstate length = " << db_2_length << "              " << std::endl;

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
            std::cout << db_2_key << std::endl;
            Address addr_buf;
            addr_buf.raw = db_2_key;
            addrdec.scriptPubKey_parse(&addr_buf);
            // std::cout << StringToHex(db_2_key) << std::endl;
            std::cout << addr_buf.print() << std::endl;
            std::cout << StringToHex(addr_buf.raw) << std::endl;
            std::cout << *(uint64_t*)db_1_value.ToString().c_str() << std::endl;
        }
        db_1.it->Next();
    }
}

void validateStates(std::string hash1, std::string hash2)
{
    Dict paths = composeJsonDict("../config/paths.json");
    std::string path1 = (paths)["cache"] + hash1;
    std::string path2 = (paths)["cache"] + hash2;

    std::string ad_path_1 = path1 + "/addressDb/";
    std::string ad_path_2 = path2 + "/addressDb/";

    std::string am_path_1 = path1 + "/amountDb/";
    std::string am_path_2 = path2 + "/amountDb/";

    std::cout << "ad_path_1 = " << ad_path_1 << std::endl;
    std::cout << "ad_path_2 = " << ad_path_2 << std::endl;
    std::cout << "am_path_1 = " << am_path_1 << std::endl;
    std::cout << "am_path_2 = " << am_path_2 << std::endl;

    std::cout << "checking addressDbs" << std::endl;
    validateDbs(ad_path_1, ad_path_2);
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "checking amountDbs" << std::endl;
    validateDbs(am_path_1, am_path_2);
}

int main()
{
    validateStates(
        "0000000000000000000528fcb7dde91b7e85aa433eb3aa1b664bc57382b30d67_1",
        "0000000000000000000528fcb7dde91b7e85aa433eb3aa1b664bc57382b30d67"
    );
}