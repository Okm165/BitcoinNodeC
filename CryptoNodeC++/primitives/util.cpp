#include "util.h"
#include "bstream.h"

Json::Value getJson(const char* path)
{
    Json::CharReaderBuilder builder;
    Json::Value json;
    std::string errors;
    std::ifstream file (path);
    Json::parseFromStream(builder, file, &json, &errors);
    return json;
}

Dict composeJsonDict(const char* path)
{
    Dict dict;
    Json::Value json = getJson(path);
    for (Json::Value::iterator it = json.begin(); it != json.end(); ++it)
        dict[it.name()] = it->asString();
    return dict;
}

std::string dent(uint8_t n)
{
    std::string string = "";
    for(uint8_t it = 0; it < n; it++)
        string += " ";
    return string;
}

char char2int(char input)
{
    if(input >= '0' && input <= '9')
        return input - '0';
    if(input >= 'A' && input <= 'F')
        return input - 'A' + 10;
    if(input >= 'a' && input <= 'f')
        return input - 'a' + 10;
    std::cout << (uint32_t)input << std::endl;
    throw std::invalid_argument("Invalid input string");
}

const char * hexALphabet = "0123456789abcdef";
Pair charToHex(unsigned char input)
{
    Pair ret(hexALphabet[input / 16], hexALphabet[input % 16]);
    return ret;
}

std::string HexToStringReversed(std::string hex)
{
    uint32_t len = hex.size();
    char buffer[len/2];
    uint32_t counter = len/2-1;
    for(std::string::iterator it = hex.begin(); it <= hex.end()-1; it+=2)
    {
        buffer[counter] = char2int(*it)*16 + char2int(*(it+1));
        counter--;
    }
    std::string ret(buffer, len/2);
    return ret;
}
std::string HexToString(std::string hex)
{
    uint32_t len = hex.size();
    char buffer[len/2];
    uint32_t counter = 0;
    for(std::string::iterator it = hex.begin(); it <= hex.end()-1; it+=2)
    {
        buffer[counter] = char2int(*it)*16 + char2int(*(it+1));
        counter++;
    }
    std::string ret(buffer, len/2);
    return ret;
}

std::string StringToHex(std::string data)
{
    uint32_t len = data.size();
    char buffer[len*2];
    uint32_t counter = 0;
    for(std::string::iterator it = data.begin(); it <= data.end()-1; it+=1)
    {
        Pair hex = charToHex(*it);
        buffer[counter++] = hex.first;
        buffer[counter++] = hex.second;
    }
    std::string ret(buffer, len*2);
    return ret;
}
std::string StringToHexReversed(std::string data)
{
    uint32_t len = data.size();
    char buffer[len*2];
    uint32_t counter = len*2-1;
    for(std::string::iterator it = data.begin(); it <= data.end()-1; it+=1)
    {
        Pair hex = charToHex(*it);
        buffer[counter--] = hex.second;
        buffer[counter--] = hex.first;
        
    }
    std::string ret(buffer, len*2);
    return ret;
}

std::string dictPrint(Dict& dict)
{
    std::string string;
    for(Dict::iterator it = dict.begin(); it != dict.end(); it++)
        string += it->first + " => " + it->second + "\n";
    return string;
}

std::string dictPrint(adDict& addict)
{
    std::string string;
    for(adDict::iterator it = addict.begin(); it != addict.end(); it++)
        string += it->first + " => " + std::to_string(it->second) + "\n";
    return string;
}

std::string dictPrint(amDict& amdict)
{
    std::string string;
    for(amDict::iterator it = amdict.begin(); it != amdict.end(); it++)
        string += std::to_string(it->first) + " => " + std::to_string(it->second) + "\n";
    return string;
}

std::string dictPrint(adChDict& adchdict)
{
    std::string string;
    for(adChDict::iterator it = adchdict.begin(); it != adchdict.end(); it++)
        string += it->first + " => " + std::to_string(it->second) + "\n";
    return string;
}

std::string dictPrint(amChDict& amchdict)
{
    std::string string;
    for(amChDict::iterator it = amchdict.begin(); it != amchdict.end(); it++)
        string += std::to_string(it->first) + " => " + std::to_string(it->second) + "\n";
    return string;
}

// dict serialization format
// uint64 dict.size() (number of rows, key and value pairs)
// char<ADDRESS.size()> <ADDRESS> int64<AMOUNT>
std::string serialize_adChDict(adChDict* dict)
{
    std::string ret;
    //determine size of adChDict in bytes
    uint64_t size = 8; //dict size variable
    for(adChDict::iterator it = dict->begin(); it != dict->end(); it++)
        size += it->first.size() + sizeof(it->second) + 1; //char before address telling how many bytes to read next
    ret.resize(size);

    BStream ser (&ret);
    ser.write((uint64_t)(dict->size()));
    for(adChDict::iterator it = dict->begin(); it != dict->end(); it++)
    {
        ser.write((unsigned char)(it->first.size()));
        memcpy(&(((char*)ser.bytes->c_str())[ser.cursor]), it->first.c_str(), it->first.size());
        ser.movePos(it->first.size());
        ser.write(it->second);
    }
    return ret;
}

adChDict deserialize_adChDict(std::string* dict)
{
    adChDict ret;
    BStream ser (dict);
    uint64_t size = ser.read<uint64_t>();
    for(uint64_t it = 0; it < size; it++)
    {
        uint8_t toread = ser.read<uint8_t>();
        std::string key ((const char *)&((ser.bytes->c_str())[ser.cursor]), toread);
        ser.movePos(toread);
        int64_t value = ser.read<int64_t>();
        ret[key] = value;
    }
    return ret;
}

std::string writeVarInt(uint32_t n)
{
    std::string ret;
    unsigned char tmp[(sizeof(n)*8+6)/7];
    int len=0;
    while(true) {
        tmp[len] = (n & 0x7F) | (len ? 0x80 : 0x00);
        if (n <= 0x7F)
            break;
        n = (n >> 7) - 1;
        len++;
    }
    do {
        ret += (char)tmp[len];
    } while(len--);
    return ret;
}

uint32_t getSpecialScriptSize(uint32_t nSize)
{
    if (nSize == 0 || nSize == 1)
        return 20;
    if (nSize == 2 || nSize == 3 || nSize == 4 || nSize == 5)
        return 32;
    return 0;
}
