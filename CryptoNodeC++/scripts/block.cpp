#include "block.h"

std::string BTxIn::print(uint8_t n)
{
    std::string string;
    string =  dent(n) + "BTxIn{\n";
    string += dent(n+1) + "txHash = " + StringToHexReversed(txHash) + "\n";
    string += dent(n+1) + "nHeight = " + std::to_string(nHeight) + "\n";
    string += dent(n+1) + "scriptSig = " + StringToHex(scriptSig) + "\n";
    string += dent(n+1) + "sequence = " + std::to_string(sequence) + "\n";
    string += dent(n+1) + "wScriptLength = " + std::to_string(witnessScriptVec.size()) + "\n";
    string += dent(n+1) + "witnessScriptVec = \n";
    for(std::string wtn : witnessScriptVec)
    {
        string += dent(n+2) + StringToHex(wtn) + "\n";
    }
    string += dent(n) + "}\n";
    return string;
}

std::string BTxOut::print(uint8_t n)
{
    std::string string;
    string =  dent(n) + "BTxOut{\n";
    string += dent(n+1) + "nHeight = " + std::to_string(nHeight) + "\n";
    string += dent(n+1) + "amount = " + std::to_string(amount) + "\n";
    string += dent(n+1) + "scriptSig = " + StringToHex(scriptSig) + "\n";
    string += dent(n) + "}\n";
    return string;
}

std::string BTx::print(uint8_t n)
{
    std::string string;
    string =  dent(n) + "BTx{\n";
    string += dent(n+1) + "hash = " + StringToHexReversed(hash) + "\n";
    string += dent(n+1) + "version = " + std::to_string(version) + "\n";
    string += dent(n+1) + "nHeight = " + std::to_string(nHeight) + "\n";
    string += dent(n+1) + "BTxInSize = " + std::to_string(inVec.size()) + "\n";
    for(BTxIn txin : inVec)
        string += txin.print(n+1);
    string += dent(n+1) + "BTxOutSize = " + std::to_string(outVec.size()) + "\n";
    for(BTxOut txout : outVec)
        string += txout.print(n+1);
    string += dent(n+1) + "nLockTime = " + std::to_string(nLockTime) + "\n";
    string += dent(n) + "}\n";
    return string;
}

std::string BBlock::print(uint8_t n)
{
    std::string string;
    string =  dent(n) + "BBlock{\n";
    string += dent(n+1) + "hash = " + StringToHexReversed(hash) + "\n";
    string += dent(n+1) + "hashPrev = " + StringToHexReversed(hashPrev) + "\n";
    string += dent(n+1) + "nHeight = " + std::to_string(nHeight) + "\n";
    string += dent(n+1) + "id = " + std::to_string(id) + "\n";
    string += dent(n+1) + "headerLength = " + std::to_string(headerLength) + "\n";
    string += dent(n+1) + "version = " + std::to_string(version) + "\n";
    string += dent(n+1) + "hashMerkleRoot = " + StringToHexReversed(hashMerkleRoot) + "\n";
    string += dent(n+1) + "nTime = " + std::to_string(nTime) + "\n";
    string += dent(n+1) + "nBits = " + std::to_string(nBits) + "\n";
    string += dent(n+1) + "nNonce = " + std::to_string(nNonce) + "\n";
    string += dent(n+1) + "BTxSize = " + std::to_string(txVec.size()) + "\n";
    for(BTx tran : txVec)
        string += tran.print(n+1);
    string += dent(n) + "}\n";
    return string;
}

std::vector<BTxIn> readInVector(BStream* bstream)
{
    uint64_t inVecLength = bstream->readCompactSize();
    std::vector<BTxIn> inVec;
    inVec.reserve(inVecLength);

    // inVec deserialization
    for(uint64_t it = 0; it < inVecLength; it++)
    {
        BTxIn txin;
        txin.txHash = bstream->read(32);
        txin.nHeight = bstream->read<uint32_t>();
        txin.scriptSig = bstream->read(bstream->readCompactSize());
        txin.sequence = bstream->read<uint32_t>();
        inVec.push_back(txin);
    }
    return inVec;
}

std::vector<BTxOut> readOutVector(BStream* bstream)
{
    uint64_t outVecLength = bstream->readCompactSize();
    std::vector<BTxOut> outVec;
    outVec.reserve(outVecLength);

    // outVec deserialization
    for(uint64_t it = 0; it < outVecLength; it++)
    {
        BTxOut txout;
        txout.nHeight = it;
        txout.amount = bstream->read<uint64_t>();
        txout.scriptSig = bstream->read(bstream->readCompactSize());
        outVec.push_back(txout);
    }
    return outVec;
}

BBlock readBBlock(Index* index, const char * blk_path, std::string& hash)
{
    IBlock iblock = readIBlock(index, hash);
    BBlock bblock;
    bblock.nHeight = iblock.nHeight;

    // compose block filePath
    std::string file_path;
    file_path += blk_path;
    file_path += "blk";
    file_path += std::to_string(100000+iblock.nFile).substr(1);
    file_path += ".dat";

    FStream blk_file(file_path.c_str(), READONLY);
    // move to nDataPos - 8 = begining of block
    blk_file.setPos(iblock.nDataPos);
    blk_file.movePos(-8);

    // parsing starts
    bblock.id = blk_file.read<uint32_t>();
    bblock.headerLength = blk_file.read<uint32_t>();

    std::string blk_stream = blk_file.read(bblock.headerLength);
    BStream bstream(&blk_stream);

    // parse block header
    bblock.version = bstream.read<uint32_t>();
    bblock.hashPrev = bstream.read(32);
    bblock.hashMerkleRoot = bstream.read(32);
    bblock.nTime = bstream.read<uint32_t>();
    bblock.nBits = bstream.read<uint32_t>();
    bblock.nNonce = bstream.read<uint32_t>();

    // compute hash
    bstream.movePos(-80);
    std::string blk_hashbuff = bstream.read(80);
    bblock.hash = doublesha256(blk_hashbuff);

    // CTransaction vector unserialization
    uint64_t txVecLength = bstream.readCompactSize();
    bblock.txVec.reserve(txVecLength);
    bool allowWitness = true; // client version dependent, for updated clients always true (after soft fork)
    
    for(uint64_t it = 0; it < txVecLength; it++)
    {
        BTx tx;
        std::string tx_hashbuff;

        tx.version = bstream.read<uint32_t>();
        bstream.movePos(-4);
        tx_hashbuff = bstream.read(4);
        tx.nHeight = it;

        // read in and out vectors
        char flags = 0;
        uint64_t tx_hashbuff_pt1 = bstream.getPos();
        tx.inVec = readInVector(&bstream);
        if(tx.inVec.size() == 0 && allowWitness)
        {
            flags = bstream.read<char>();
            if(flags != 0)
            {
                uint64_t tx_hashbuff_pt1 = bstream.getPos();
                tx.inVec = readInVector(&bstream);
                tx.outVec = readOutVector(&bstream);
                uint64_t tx_hashbuff_pt2 = bstream.getPos();

                bstream.setPos(tx_hashbuff_pt1);
                tx_hashbuff += bstream.read(tx_hashbuff_pt2-tx_hashbuff_pt1);
            }
        }
        else
        {
            tx.outVec = readOutVector(&bstream);
            uint64_t tx_hashbuff_pt2 = bstream.getPos();

            bstream.setPos(tx_hashbuff_pt1);
            tx_hashbuff += bstream.read(tx_hashbuff_pt2-tx_hashbuff_pt1);
        }

        // read witness
        if((flags & 1) && allowWitness)
        {
            flags ^= 1;
            for(BTxIn& txin: tx.inVec)
            {
                uint64_t wit_len = bstream.readCompactSize();
                for(size_t it = 0; it < wit_len; it++)
                {
                    uint64_t wit_item_len = bstream.readCompactSize();
                    txin.witnessScriptVec.push_back(bstream.read(wit_item_len));
                }
            }
        }

        // nLockTime
        tx.nLockTime = bstream.read<uint32_t>();
        bstream.movePos(-4);
        tx_hashbuff += bstream.read(4);
        tx.hash = doublesha256(tx_hashbuff);
        bblock.txVec.push_back(tx);
    }
    return bblock;
}