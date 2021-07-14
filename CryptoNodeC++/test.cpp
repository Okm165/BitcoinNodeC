#include "scripts/scripts.h"

int main()
{
    Dict paths = composeJsonDict("config/paths.json");

    // init
    Chain chain (paths["chainstate"].c_str());
    OpCodes opcodes (paths["opcodes"].c_str());
    Index index (paths["index"].c_str());

    std::string hash(HexToStringReversed("00000000000000000003dddd6117d276b69a6ffa513c1c221494bda622fdd1b7"));

    readIBlock(index, hash);
    readRBlock(index, opcodes, paths["blocks"].c_str(), hash);
    readBBlock(index, opcodes, paths["blocks"].c_str(), hash);
    readCBlock(chain, opcodes);
}