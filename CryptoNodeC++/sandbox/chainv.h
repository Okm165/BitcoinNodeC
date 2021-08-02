/* 
Chain Verifier functionalty --> verify two states 
*/

#ifndef CHAINV_H
#define CHAINV_H

#include "../scripts/scripts.h"

void validateDbs(std::string& path_1, std::string& path_2);

void validateStates(std::string hash1, std::string hash2);

#endif