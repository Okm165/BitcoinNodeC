#include "util.h"

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
    {
        dict[it.name()] = it->asString();
    }
    return dict;
}

std::string dent(int n)
{
    std::string string = "";
    for(int it = 0; it < n; it++){string += " ";}
    return string;
}

template<class T>
char sign(T num)
{
    if(num > 0){return 1;}
    else if(num == 0){return 0;}
    else{return -1;}
}

char char2int(char input)
{
    if(input >= '0' && input <= '9')
        return input - '0';
    if(input >= 'A' && input <= 'F')
        return input - 'A' + 10;
    if(input >= 'a' && input <= 'f')
        return input - 'a' + 10;
    std::cout << input << std::endl;
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