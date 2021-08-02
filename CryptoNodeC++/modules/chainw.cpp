#include "chainw.h"

void ChainW::title()
{
//    std::cout << "||           _           _                     ||" << std::endl;
//    std::cout << "||          | |         (_)                    ||" << std::endl;
//    std::cout << "||       ___| |__   __ _ _ _ ____      __      ||" << std::endl;
//    std::cout << "||      / __| '_ \\ / _` | | '_ \\ \\ /\\ / /      ||" << std::endl;
//    std::cout << "||     | (__| | | | (_| | | | | \\ V  V /       ||" << std::endl;
//    std::cout << "||      \\___|_| |_|\\__,_|_|_| |_|\\_/\\_/        ||" << std::endl;
//    std::cout << std::endl;
   std::cout << "chain walker" << std::endl;
}

void ChainW::compose_adChDict(std::string& hash)
{
    adchdict.clear();

    RBlock rblock = readRBlock(index, addrdec, (*paths)["blocks"].c_str(), hash);   
    for(std::vector<RTxIn>::iterator it = rblock.inVec.begin(); it != rblock.inVec.end(); it++)
        if(it->address.addressFlag && it->address.type != FAIL)
        {
            if(adchdict.find(it->address.address) != adchdict.end())
            {
                int64_t newval = adchdict[it->address.address] - (it->amount);
                if(newval == 0){adchdict.erase(it->address.address);} 
                else{adchdict[it->address.address] = newval;}
            }
            else{adchdict[it->address.address] = -(it->amount);}
        }
    
    BBlock bblock = readBBlock(index, addrdec, (*paths)["blocks"].c_str(), hash);
    for(std::vector<BTx>::iterator it = bblock.txVec.begin(); it != bblock.txVec.end(); it++)
        for(std::vector<BTxOut>::iterator itin = it->outVec.begin(); itin != it->outVec.end(); itin++)
            if(itin->address.addressFlag && itin->address.type != FAIL)
            {
                if(adchdict.find(itin->address.address) != adchdict.end())
                {
                    int64_t newval = adchdict[itin->address.address] + itin->amount;
                    if(newval == 0){adchdict.erase(itin->address.address);} 
                    else{adchdict[itin->address.address] = newval;}
                }
                else{adchdict[itin->address.address] = (itin->amount);}
            }
}

void ChainW::compose_amChDict()
{
    amchdict.clear();
    if(progress){bar = new ProgressBar("compose_amChDict...", tadChDict.size(), PROGRESS_BAR_SETTINGS);}
    for(adChDict::iterator it = tadChDict.begin(); it != tadChDict.end(); it++)
    {
        leveldb::Status status;
        leveldb::Slice db_key(it->first);

        std::string db_val_ret;

        status = startHashDb->Get(leveldb::ReadOptions(), db_key, &db_val_ret);
        assert(!status.IsIOError());

        if(status.IsNotFound())
        {
            uint64_t key = abs(it->second);
            //dictWrite
            if(amchdict.find(key) != amchdict.end())
            {
                // amchdict[key] = amchdict[key] + 1;
                int64_t newval = amchdict[key] + 1;
                if(newval == 0){amchdict.erase(key);} 
                else{amchdict[key] = newval;}
            }
            else{amchdict[key] = 1;}
        }
        else
        {
            uint64_t value = *(uint64_t *)db_val_ret.data();
            //dictWrite
            if(amchdict.find(value) != amchdict.end())
            {
                // amchdict[value] = amchdict[value] - 1;
                int64_t newval = amchdict[value] - 1;
                if(newval == 0){amchdict.erase(value);} 
                else{amchdict[value] = newval;}
            }
            else{amchdict[value] = -1;}

            int64_t sum = value + (it->second)*direction;

            if(!sum)
            {
                if(progress){bar->update();}
                continue;
            }

            uint64_t key = abs(sum);
            //dictWrite
            if(amchdict.find(key) != amchdict.end())
            {
                // amchdict[key] = amchdict[key] + 1;
                int64_t newval = amchdict[key] + 1;
                if(newval == 0){amchdict.erase(key);} 
                else{amchdict[key] = newval;}
            }
            else{amchdict[key] = 1;}
        }
        if(progress){bar->update();}
    }
    if(progress){bar->close();}
    if(progress){delete bar;}
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

void ChainW::composeHashPath()
{
    if(progress){std::cout << "composeHashPath...  " << std::flush;}
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
    if(progress){std::cout << "DONE" << "                    " << std::endl;}
}

void ChainW::composeState(std::string startHash, std::string dstHash)
{
    this->dstHash = dstHash;
    this->startHash = startHash;
    dsthashpath = (*paths)["cache"] + StringToHexReversed(dstHash) + "/";
    if(dirExists(dsthashpath.c_str()) && !override){return;}

    composeHashPath();

    starthashpath = (*paths)["cache"] + StringToHexReversed(startHash) + "/";

    if(progress){bar = new ProgressBar("compose_adChDict...", hashPath.size(), PROGRESS_BAR_SETTINGS);}
    tadChDict.clear();
    for(std::vector<std::string>::iterator it = hashPath.begin(); it != hashPath.end(); it++)
    {
        std::string itpath = (*paths)["cache"] + StringToHexReversed(*it) + ".dict";
        if(!fileExists(itpath.c_str()))
        {
            compose_adChDict(*it);
            save_adChDict(&adchdict, itpath);
        }
        else
        {
            load_adChDict(&adchdict, itpath);
        }
        for(adChDict::iterator it = adchdict.begin(); it != adchdict.end(); it++)
        {
            //dictWrite
            if(tadChDict.find(it->first) != tadChDict.end())
            {
                int64_t newval = tadChDict[it->first] + it->second;
                if(newval == 0){tadChDict.erase(it->first);}
                else{tadChDict[it->first] = newval;}
            }
            else{tadChDict[it->first] = it->second;}
        }
        if(progress){bar->update();}
    }
    if(progress){bar->close();}
    if(progress){delete bar;}

    std::cout << tadChDict.size() << std::endl;

    std::string itpath = (*paths)["cache"] + "final" + ".dict";
    save_adChDict(&adchdict, itpath);

    std::string startHashDbPath = starthashpath + "addressDb/";
    LevelDb startHashDbObj(startHashDbPath.c_str());
    startHashDb = startHashDbObj.db;

    compose_amChDict();

    std::cout << amchdict.size() << std::endl;
    
    if(progress){std::cout << "copyDir...  " << std::flush;}
    copyDir(starthashpath, dsthashpath);
    if(progress){std::cout << "DONE" << "                    " << std::endl;}

    std::string dst_adDbpath = dsthashpath + "addressDb";
    std::string dst_amDbpath = dsthashpath + "amountDb";

    LevelDb adDb(dst_adDbpath.c_str());
    LevelDb amDb(dst_amDbpath.c_str());

    if(progress){bar = new ProgressBar("marking addressdb...", tadChDict.size(), PROGRESS_BAR_SETTINGS);}
    for(adChDict::iterator it = tadChDict.begin(); it != tadChDict.end(); it++)
    {
        // adDb.update()
        std::string key = it->first;
        int64_t tmp = (it->second)*(direction);
        std::string value ((const char*)&(tmp), sizeof(int64_t));
        adDb.update(key, value);
        if(progress){bar->update();}
    }
    if(progress){bar->close();}
    if(progress){delete bar;}

    if(progress){bar = new ProgressBar("marking amountdb...", amchdict.size(), PROGRESS_BAR_SETTINGS);}
    for(amChDict::iterator it = amchdict.begin(); it != amchdict.end(); it++)
    {
        // amDb.update()
        std::string key ((const char*)&(it->first), sizeof(int64_t));
        std::string value ((const char*)&(it->second), sizeof(int64_t));
        amDb.update(key, value);
        if(progress){bar->update();}
    }
    if(progress){bar->close();}
    if(progress){delete bar;}
}

void ChainW::validateStates(std::string hash1, std::string hash2)
{
    std::string path1 = (*paths)["cache"] + hash1;
    std::string path2 = (*paths)["cache"] + hash2;

    std::string ad_path_1 = path1 + "/addressDb/";
    std::string ad_path_2 = path2 + "/addressDb/";
    std::string am_path_1 = path1 + "/amountDb/";
    std::string am_path_2 = path2 + "/amountDb/";

    if(progress)
    {
        std::cout << "ad_path_1 = " << ad_path_1 << std::endl;
        std::cout << "ad_path_2 = " << ad_path_2 << std::endl;
        std::cout << "am_path_1 = " << am_path_1 << std::endl;
        std::cout << "am_path_2 = " << am_path_2 << std::endl;
    }
    std::cout << "checking addressDbs" << std::endl;
    compareDbs(ad_path_1, ad_path_2, progress);
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "checking amountDbs" << std::endl;
    compareDbs(am_path_1, am_path_2, progress);
}

int main()
{
    Dict paths = composeJsonDict("../config/paths.json");
    Index index(paths["index"].c_str());
    AddressDecoder addrdec;
    ChainW chainw(&paths, &index, &addrdec, true, true);
    chainw.title();

    chainw.composeState(
        HexToStringReversed("00000000000000000006af3132ae7e718adc5f598480e8bbefd198047dba2eac"),
        HexToStringReversed("00000000000000000001ad9bf7fbb09a96099863f2d3d50321a7ba0eb50021be")
    );

    chainw.validateStates(
        "00000000000000000001ad9bf7fbb09a96099863f2d3d50321a7ba0eb50021be_1",
        "00000000000000000001ad9bf7fbb09a96099863f2d3d50321a7ba0eb50021be"
    );
}