#include "../scripts/scripts.h"

int main()
{
    Dict paths = composeJsonDict("../config/paths.json");
    Index index(paths["index"].c_str());
    AddressDecoder addrdec;
    std::string hash (HexToStringReversed("000000000000000000129da18d7db6b158d55b23f7386fe8dd6b7cfdb90de71a"));
    BBlock bblock = readBBlock(&index, &addrdec, paths["blocks"].c_str(), hash);
    std::cout << bblock.print() << std::endl;
    RBlock rblock = readRBlock(&index, &addrdec, paths["blocks"].c_str(), hash);
    std::cout << rblock.print() << std::endl;
}