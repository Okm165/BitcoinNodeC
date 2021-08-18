/*
          _           _               
         | |         (_)              
      ___| |__   __ _ _ _ ____      __
     / __| '_ \ / _` | | '_ \ \ /\ / /
    | (__| | | | (_| | | | | \ V  V / 
     \___|_| |_|\__,_|_|_| |_|\_/\_/  


Chain Walker functionalty --> generate new state based on parsed state
------------------------------------------------------------------------------------------
In order to use walker create object ChainW and call composeState() functionality
set progress to true if you want to display progress bars,
set override to true if you want to ignore shorter solutions based on cache, (set to false if cache is corrupted)
Walker will generate new state and save it in path spacified by ("cache": ?) in config/paths.json
it is useful to use Walker because it is involves less amount of computation then parser (much faster)
------------------------------------------------------------------------------------------
void composeState(std::string& startHash, std::string& dstHash)
Composes new state, the same as parser would generate for <dstHash>, 
it starts from <startHash> state and walks over all blocks between to get to target <dstHash>
in the mean time it stores all changes that those blocks bring
in addressDict as well as amountDict, after succeful walk original
<startHash> state is copied and all necessary changes are applied to both addressDb and amountDb,
to get dstHash state, new dstHash state is saved under <dstHash> name
in path spacified by ("cache": ?) in config/paths.json
------------------------------------------------------------------------------------------
void compose_adChDict(std::string& hash)
Compose address change dictionary
------------------------------------------------------------------------------------------
void compose_amChDict(adChDict* adChDict)
Compose amount change dictionary
------------------------------------------------------------------------------------------
void load_adChDict(adChDict* dict, std::string& path)
Load <adChDict> from cache
------------------------------------------------------------------------------------------
void save_adChDict(adChDict* dict, std::string& path)
Save <adChDict> to cache
------------------------------------------------------------------------------------------
void composeHashPath()
Compose block path from <startHash> to dstHash, find shortest path if override is false
------------------------------------------------------------------------------------------
void validateStates()
------------------------------------------------------------------------------------------
*/

#ifndef CHAINW_H
#define CHAINW_H

#include "../scripts/scripts.h"

class ChainW
{
    protected:

    Dict* paths;
    Index* index;
    AddressDecoder* addrdec;

    adChDict adchdict;
    amChDict amchdict;
    char direction = 0;                         // 1 if moving forward -1 if moving backward

    std::string startHash;
    std::string dstHash;
    std::string starthashpath;
    std::string dsthashpath;

    LevelDb* startHashDb;                       // start hash database, (for compose_amChDict functionality)
    std::vector<std::string> hashPath;          // vector consiting hashes to go through in order to get to desired state
    bool override;                              // if true chainwalker won't consider shorter paths, only absolute from startHash to dstHash
    ProgressBar* bar;

    public:

    ChainW(){};
    ChainW(Dict* paths, Index* index, AddressDecoder* addrdec, bool override = false)
    {
        this->paths = paths;
        this->index = index;
        this->addrdec = addrdec;
        this->override = override;
        title();
    }
    ~ChainW(){delete startHashDb;}
    void title();

    void compose_adChDict();
    void compose_amChDict();
    void load_adChDict(adChDict* dict, std::string& path);
    void save_adChDict(adChDict* dict, std::string& path);
    void compose_hashPath();
    
    void composeState(std::string startHash, std::string dstHash);
    void validateStates(std::string hash1, std::string hash2);

};

#endif