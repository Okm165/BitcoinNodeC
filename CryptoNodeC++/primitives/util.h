/*
Utilities
------------------------------------------------------------------------------------------
*/

#ifndef UTIL_H
#define UTIL_H

#include <map>
#include <string>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <iostream>
#include "json/json.h"

#define PROGRESS_BAR_SETTINGS 100, 0.005, 50

typedef std::pair<std::string, std::string> Row;
typedef std::pair<char, char> Pair;
typedef std::map<std::string, std::string> Dict;                    // dict

typedef std::map<std::string, uint64_t> adDict;                     // address dict
typedef std::map<uint64_t, uint64_t> amDict;                        // amount dict

typedef std::map<std::string, int64_t> adChDict;                    // address change dict
typedef std::map<uint64_t, int64_t> amChDict;                       // amount change dict

Json::Value getJson(const char* path);
Dict composeJsonDict(const char* path);

std::string dent(uint8_t n);

template<class T>
char sign(const T& num)
{
    if(num > 0){return 1;}
    else if(!num){return 0;}
    else{return -1;}
}

char char2int(char input);
Pair charToHex(unsigned char input);

std::string HexToStringReversed(std::string hex);
std::string HexToString(std::string hex);

std::string StringToHex(std::string data);
std::string StringToHexReversed(std::string data);

std::string dictPrint(Dict& dict);
std::string dictPrint(adDict& addict);
std::string dictPrint(amDict& amdict);
std::string dictPrint(adChDict& adchdict);
std::string dictPrint(amChDict& adchdict);

template<class Dict, class Key, class Value>
void dictWrite(Dict* dict, const Key& key, const Value& value)
{
    if(dict->find(key) != dict->end())
    {
        int64_t newval = (*dict)[key] + value;
        if(!newval)
            dict->erase(key);
        else
            (*dict)[key] = newval;
    }
    else
    {
        if(value)
            (*dict)[key] = value;
    }
        
}

std::string serialize_adChDict(adChDict* dict);
adChDict deserialize_adChDict(std::string* dict);

// read variable length integer
std::string writeVarInt(uint32_t n);

// get special script size
uint32_t getSpecialScriptSize(uint32_t nSize);

// switch endian functionality
template <class T>
T switchEndian(T &num)
{
    char reversed[sizeof(T)];
    for(uint32_t it = 0; it < sizeof(T); it++)
        reversed[it] = ((char*)(&num))[sizeof(T)-1-it];

    return *(T*)reversed;
}

#endif