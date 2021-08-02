#include "bstream.h"

BStream::BStream(std::string* data, uint64_t cursor)
{
    this->bytes = data;
    this->cursor = cursor;
}

//get current position in bytestream
uint64_t BStream::getPos(){return cursor;}

//set absolute position in bytestream
void BStream::setPos(uint64_t pos){cursor = pos;}

//move cursor relatively to current position
void BStream::movePos(uint64_t step){cursor += step;}

//get length
uint64_t BStream::getLength(){return bytes->size();}

//read length of bytes
std::string BStream::read(uint64_t length)
{
    if(cursor+length > bytes->size())
        length = bytes->size()-cursor;
    std::string ret(&(bytes->data()[cursor]), length);
    movePos(length);
    return ret;
}

//reset cursor to 0
void BStream::reset(){cursor = 0;}

//read compact size
uint64_t BStream::readCompactSize()
{
    unsigned char chSize = read<char>();
    uint64_t nSize = 0;
    if(chSize < 253){nSize = chSize;}
    else if(chSize == 253){nSize = read<short>();}
    else if(chSize == 254){nSize = read<int>();}
    else{nSize = read<long int>();}
    return nSize;
}

//read all bites till end
std::string BStream::readToEnd()
{   
    uint64_t len = getLength() - cursor;
    std::string ret(&bytes->data()[cursor]);
    movePos(len);
    return ret;
}

//read variable length integer
uint64_t BStream::readVarInt()
{
    uint64_t n = 0;
    while(true)
    {
        unsigned char chData = read<unsigned char>();
        n = (n << 7) | (chData & 0x7F);
        if(chData & 0x80){n++;}
        else{return n;}
    }
}

uint64_t BStream::decompressAmount(uint64_t x)
{
    // x = 0  OR  x = 1+10*(9*n + d - 1) + e  OR  x = 1+10*(n - 1) + 9
    if (x == 0)
        return 0;
    x--;
    // x = 10*(9*n + d - 1) + e
    int e = x % 10;
    x /= 10;
    uint64_t n = 0;
    if (e < 9) {
        // x = 9*n + d - 1
        int d = (x % 9) + 1;
        x /= 9;
        // x = n
        n = x*10 + d;
    } else {
        n = x+1;
    }
    while (e) {
        n *= 10;
        e--;
    }
    return n;
}