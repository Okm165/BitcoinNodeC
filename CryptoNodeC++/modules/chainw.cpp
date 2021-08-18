#include "chainw.h"

void ChainW::title(){std::cout << "chain walker" << std::endl;}

void ChainW::compose_adChDict()
{
    adchdict.clear();
    adChDict tmp;
    ProgressBar bar ("composing adChDict", hashPath.size(), PROGRESS_BAR_SETTINGS);
    for(std::vector<std::string>::iterator it = hashPath.begin(); it != hashPath.end(); it++)
    {
        tmp.clear();
        std::string itpath = (*paths)["cache"] + StringToHexReversed(*it) + ".dict";
        if(!fileExists(itpath.c_str()))
        {
            BBlock bblock = readBBlock(index, (*paths)["blocks"].c_str(), *it);
            for(std::vector<BTx>::iterator bblock_it = bblock.txVec.begin(); bblock_it != bblock.txVec.end(); bblock_it++)
                for(std::vector<BTxOut>::iterator bblock_itout = bblock_it->outVec.begin(); bblock_itout != bblock_it->outVec.end(); bblock_itout++)
                {
                    Address address;
                    addrdec->addressDecode(&address, &bblock_itout->scriptSig, SCRIPT);
                    if(address.IsValid())
                        dictWrite(&tmp, address.address, (int64_t)bblock_itout->amount);
                }

            RBlock rblock = readRBlock(index, (*paths)["blocks"].c_str(), *it);
            for(std::vector<RTxIn>::iterator rblock_it = rblock.inVec.begin(); rblock_it != rblock.inVec.end(); rblock_it++)
            {
                Address address;
                addrdec->addressDecode(&address, &rblock_it->scriptSig, TYPE);
                if(address.IsValid())
                    dictWrite(&tmp, address.address, -(int64_t)rblock_it->amount);
            }

            save_adChDict(&tmp, itpath);
        }
        else
            load_adChDict(&tmp, itpath);

        for(adChDict::iterator tmp_it = tmp.begin(); tmp_it != tmp.end(); tmp_it++)
            dictWrite(&adchdict, tmp_it->first, tmp_it->second);

        bar.update();
    }
    bar.close();
}

void ChainW::compose_amChDict()
{
    amchdict.clear();
    ProgressBar bar ("composing amChDict", adchdict.size(), PROGRESS_BAR_SETTINGS);
    for(adChDict::iterator it = adchdict.begin(); it != adchdict.end(); it++)
    {
        leveldb::Status status;
        leveldb::Slice db_key(it->first);

        std::string db_val_ret;

        status = startHashDb->db->Get(leveldb::ReadOptions(), db_key, &db_val_ret);
        assert(!status.IsIOError());

        if(status.IsNotFound())
        {
            uint64_t key = abs(it->second);
            if(!key)
            {
                bar.update();
                continue;
            }
            dictWrite(&amchdict, key, 1);
        }
        else
        {
            uint64_t value = *(uint64_t *)db_val_ret.data();
            dictWrite(&amchdict, value, -1);

            int64_t sum = value + (it->second)*direction;
            if(!sum)
            {
                bar.update();
                continue;
            }

            uint64_t key = abs(sum);
            dictWrite(&amchdict, key, 1);
        }
        bar.update();
    }
    bar.close();
}

void ChainW::load_adChDict(adChDict* dict, std::string& path)
{
    dict->clear();
    FStream file(path.c_str(), READONLY);
    std::string content = file.read(file.length);
    *dict = deserialize_adChDict(&content);
    file.close();
}

void ChainW::save_adChDict(adChDict* dict, std::string& path)
{
    std::string data = serialize_adChDict(dict);
    FStream file(path.c_str(), WRITEONLY);
    file.write(data);
    file.close();
}

void ChainW::compose_hashPath()
{
    std::cout << "composing hashpath" << std::endl;
    hashPath.clear();
    std::string newStartHash = startHash;

    direction = sign((int64_t)(readIBlock(index, dstHash).nHeight - readIBlock(index, startHash).nHeight));
    if(!direction){return;}
    else if(direction == 1)
    {
        while(startHash != dstHash)
        {
            std::string path = (*paths)["cache"] + StringToHexReversed(dstHash) + "/";
            if(dirExists(path.c_str()) && !override)
            {
                newStartHash = dstHash;
                break;
            }
            hashPath.insert(hashPath.begin(), dstHash);
            dstHash = readIBlock(index, dstHash).hashPrev;
        }
    }
    else
    {
        while(startHash != dstHash)
        {
            std::string path = (*paths)["cache"] + StringToHexReversed(startHash) + "/";
            if(dirExists(path.c_str()) && !override)
            {
                newStartHash = startHash;
                hashPath.clear();
            }
            hashPath.push_back(startHash);
            startHash = readIBlock(index, startHash).hashPrev;
        }
    }
    startHash = newStartHash;
}

void ChainW::composeState(std::string startHash, std::string dstHash)
{
    this->dstHash = dstHash;
    this->startHash = startHash;
    starthashpath = (*paths)["cache"] + StringToHexReversed(startHash) + "/";
    dsthashpath = (*paths)["cache"] + StringToHexReversed(dstHash) + "/";
    if(dirExists(dsthashpath.c_str()) && !override){return;}

    std::string startHashDbPath = starthashpath + "addressDb/";
    startHashDb = new LevelDb (startHashDbPath.c_str());

    compose_hashPath();
    compose_adChDict();
    compose_amChDict();
    
    std::string total((*paths)["cache"] + "total.dict");
    save_adChDict(&adchdict, total);

    std::cout << "forking state" << std::endl;
    copyDir(starthashpath, dsthashpath);

    std::string dst_adDbpath = dsthashpath + "addressDb";
    std::string dst_amDbpath = dsthashpath + "amountDb";
    LevelDb dst_adDb(dst_adDbpath.c_str());
    LevelDb dst_amDb(dst_amDbpath.c_str());

    ProgressBar bar_ad ("applying adChDict", adchdict.size(), PROGRESS_BAR_SETTINGS);
    for(adChDict::iterator it = adchdict.begin(); it != adchdict.end(); it++)
    {
        std::string key = it->first;
        int64_t tmp = (it->second)*(direction);
        std::string value ((const char*)&(tmp), sizeof(int64_t));
        dst_adDb.update(key, value);
        bar_ad.update();
    }
    bar_ad.close();

    ProgressBar bar_am ("applying amChDict", amchdict.size(), PROGRESS_BAR_SETTINGS);
    for(amChDict::iterator it = amchdict.begin(); it != amchdict.end(); it++)
    {
        std::string key ((const char*)&(it->first), sizeof(int64_t));
        std::string value ((const char*)&(it->second), sizeof(int64_t));
        dst_amDb.update(key, value);
        bar_am.update();
    }
    bar_am.close();
}

void ChainW::validateStates(std::string hash1, std::string hash2)
{
    std::string path1 = (*paths)["cache"] + hash1;
    std::string path2 = (*paths)["cache"] + hash2;

    std::string ad_path_1 = path1 + "/addressDb/";
    std::string ad_path_2 = path2 + "/addressDb/";
    std::string am_path_1 = path1 + "/amountDb/";
    std::string am_path_2 = path2 + "/amountDb/";

    std::cout << "ad_path_1 = " << ad_path_1 << std::endl;
    std::cout << "ad_path_2 = " << ad_path_2 << std::endl;
    std::cout << "am_path_1 = " << am_path_1 << std::endl;
    std::cout << "am_path_2 = " << am_path_2 << std::endl;

    std::cout << "checking addressDbs" << std::endl;
    compareDbs(ad_path_1, ad_path_2);
    std::cout << "checking amountDbs" << std::endl;
    compareDbs(am_path_1, am_path_2);
}

int main()
{
    Dict paths = composeJsonDict("../config/paths.json");
    Index index(paths["index"].c_str());
    AddressDecoder addrdec;
    ChainW chainw(&paths, &index, &addrdec, true);

    chainw.composeState(
        HexToStringReversed("00000000000000000006af3132ae7e718adc5f598480e8bbefd198047dba2eac"),
        HexToStringReversed("0000000000000000000ded665b985a4000622a8529ea05cc0b395cefd894c659")
    );

    chainw.validateStates(
        "0000000000000000000ded665b985a4000622a8529ea05cc0b395cefd894c659_1",
        "0000000000000000000ded665b985a4000622a8529ea05cc0b395cefd894c659"
    );
}