#include "index.h"

std::string IBlock::print(int n)
{
    std::string string;
    string =  dent(n) + "IBlock{\n";
    string += dent(n+1) + "nVersion = " + std::to_string(nVersion) + "\n";
    string += dent(n+1) + "nHeight = " + std::to_string(nHeight) + "\n";
    string += dent(n+1) + "nStatus = " + std::to_string(nStatus) + "\n";
    string += dent(n+1) + "nTx = " + std::to_string(nTx) + "\n";
    string += dent(n+1) + "nFile = " + std::to_string(nFile) + "\n";
    string += dent(n+1) + "nDataPos = " + std::to_string(nDataPos) + "\n";
    string += dent(n+1) + "nUndoPos = " + std::to_string(nUndoPos) + "\n";
    string += dent(n+1) + "version = " + std::to_string(version) + "\n";
    string += dent(n+1) + "hashPrev = " + StringToHexReversed(hashPrev) + "\n";
    string += dent(n+1) + "hashMerkleRoot = " + StringToHexReversed(hashMerkleRoot) + "\n";
    string += dent(n+1) + "nTime = " + std::to_string(nTime) + "\n";
    string += dent(n+1) + "nBits = " + std::to_string(nBits) + "\n";
    string += dent(n+1) + "nNonce = " + std::to_string(nNonce) + "\n";
    string += dent(n) + "}\n";
    return string;
}

IBlock readIBlock(Index& index, std::string& hash)
{
    std::string key = 'b' + hash;
    std::string value = index.get(key);
    BStream b_value(value);
    IBlock iblock;
    iblock.nVersion = (int64_t)b_value.readVarInt();
    iblock.nHeight = b_value.readVarInt();
    iblock.nStatus = b_value.readVarInt();
    iblock.nTx = b_value.readVarInt();
    iblock.nFile = b_value.readVarInt();
    iblock.nDataPos = b_value.readVarInt();
    iblock.nUndoPos = b_value.readVarInt();
    iblock.version = b_value.read<int>();
    iblock.hashPrev = b_value.read(32);
    iblock.hashMerkleRoot = b_value.read(32);
    iblock.nTime = b_value.read<uint32_t>();
    iblock.nBits = b_value.read<uint32_t>();
    iblock.nNonce = b_value.read<uint32_t>();
    return iblock;
}