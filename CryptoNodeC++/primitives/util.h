#ifndef UTIL_H
#define UTIL_H

#include <map>
#include <string>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <iostream>
#include "json/json.h"

typedef std::pair<std::string, std::string> Row;
typedef std::pair<char, char> Pair;
typedef std::map<unsigned char, std::string> OpDict;
typedef std::map<std::string, std::string> Dict;

Json::Value getJson(const char* path);

Dict composeJsonDict(const char* path);

std::string dent(int n);

template<class T>
char sign(T num);

char char2int(char input);

Pair charToHex(unsigned char input);

std::string HexToStringReversed(std::string hex);
std::string HexToString(std::string hex);

std::string StringToHex(std::string data);
std::string StringToHexReversed(std::string data);

#endif