#include "rev.h"

std::string RTxIn::print(uint8_t n)
{
    std::string string;
    string =  dent(n) + "RTxIn{\n";
    string += dent(n+1) + "txHeight = " + std::to_string(txHeight) + "\n";
    string += dent(n+1) + "nHeight = " + std::to_string(nHeight) + "\n";
    string += dent(n+1) + "isCoinBase = " + std::to_string(isCoinBase) + "\n";
    string += dent(n+1) + "version = " + std::to_string(version) + "\n";
    string += dent(n+1) + "amount = " + std::to_string(amount) + "\n";
    string += address.print(n+1);
    string += dent(n) + "}\n";
    return string;
}

std::string RBlock::print(uint8_t n)
{
    std::string string;
    string =  dent(n) + "RBlock{\n";
    string += dent(n+1) + "hash = " + StringToHexReversed(hash) + "\n";
    string += dent(n+1) + "hashPrev = " + StringToHexReversed(hashPrev) + "\n";
    string += dent(n+1) + "nHeight = " + std::to_string(nHeight) + "\n";
    string += dent(n+1) + "id = " + std::to_string(id) + "\n";
    string += dent(n+1) + "headerLength = " + std::to_string(headerLength) + "\n";
    string += dent(n+1) + "nFile = " + std::to_string(nFile) + "\n";
    string += dent(n+1) + "nDataPos = " + std::to_string(nDataPos) + "\n";
    string += dent(n+1) + "nUndoPos = " + std::to_string(nUndoPos) + "\n";
    string += dent(n+1) + "RTxInSize = " + std::to_string(inVec.size());
    for(RTxIn txin : inVec)
        string += txin.print(n+1);
    string += dent(n+1) + "checksum = " + StringToHex(checksum) + "\n";
    string += dent(n) + "}\n";
    return string;
}

RBlock readRBlock(Index* index, AddressDecoder* addrdec, const char* rev_path, std::string& hash)
{
    IBlock iblock = readIBlock(index, hash);
    RBlock rblock;

    rblock.hash = hash;
    rblock.hashPrev = iblock.hashPrev;
    rblock.nHeight = iblock.nHeight;
    rblock.nFile = iblock.nFile;
    rblock.nDataPos = iblock.nDataPos;
    rblock.nUndoPos = iblock.nUndoPos;

    // compose block filePath
    std::string file_path;
    file_path += rev_path;
    file_path += "rev";
    file_path += std::to_string(100000+iblock.nFile).substr(1);
    file_path += ".dat";
    FStream rev_file(file_path.c_str(), READONLY);
    // move to nUndoPos - 8 = begining of block
    rev_file.setPos(iblock.nUndoPos);
    rev_file.movePos(-8);
    // parsing starts
    rblock.id = rev_file.read<int>();
    rblock.headerLength = rev_file.read<uint32_t>();
    
    std::string rev_stream = rev_file.read(rblock.headerLength);
    BStream bstream(&rev_stream);

    // vector unserialization
    uint64_t inVecLength = bstream.readCompactSize();
    rblock.inVec.reserve(inVecLength);

    for(size_t it = 0; it < inVecLength; it++)
    {
        uint64_t num = bstream.readCompactSize();
        for(size_t n = 0; n < num; n++)
        {
            RTxIn txin;
            txin.txHeight = it;
            txin.nHeight = n;
            uint64_t code = bstream.readVarInt();
            txin.isCoinBase = (bool)(code & 1);
            uint64_t height = code >> 1;
            if(height > 0)
                txin.version = bstream.readVarInt();
            txin.amount = bstream.decompressAmount(bstream.readVarInt());
            addrdec->addressDecode(&txin.address, &bstream, TYPE);
            rblock.inVec.push_back(txin);
        }
    }
    rblock.checksum = rev_file.read(32);
    return rblock;
}