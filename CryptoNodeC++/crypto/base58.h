#ifndef BASE58_H
#define BASE58_H

#include <string>
#include <vector>
#include <cassert>

std::string EncodeBase58SUB(const unsigned char* pbegin, const unsigned char* pend);
std::string EncodeBase58(const std::string& vch);

#endif