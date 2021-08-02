/*
BStream functionalty --> bytestream (bytearray)
------------------------------------------------------------------------------------------
In order to use BStream create object BStream
------------------------------------------------------------------------------------------
BStream(std::string* data, uint64_t cursor)
<data> is string of data to be loaded to bstream object,
cursor position set to <cursor>, but can be freely moved,
bytestream serves bytearray functionalities
------------------------------------------------------------------------------------------
*/

#ifndef BSTREAM_H
#define BSTREAM_H

#include "util.h"

class BStream
{
    public:

    std::string* bytes;
    uint64_t cursor;

    BStream(std::string* data, uint64_t cursor=0);

    //get current position in bytestream
    uint64_t getPos();

    //set absolute position in bytestream
    void setPos(uint64_t pos);

    //move cursor relatively to current position
    void movePos(uint64_t step);

    //get length
    uint64_t getLength();

    //read bytes
    template<class T>
    T read()
    {
        if(cursor+sizeof(T) > bytes->size())
            return 0;
        T ret = *(T *)&(bytes->data()[cursor]);
        movePos(sizeof(T));
        return ret;
    }
    
    //read length of bytes
    std::string read(uint64_t length);

    //reset cursor to 0
    void reset();

    //read compact size
    uint64_t readCompactSize();

    //read all bites till end
    std::string readToEnd();

    //read variable length integer
    uint64_t readVarInt();

    // amount decompress
    uint64_t decompressAmount(uint64_t x);
    
    template<class T>
    void write(T insert)
    {
        memcpy(&(((char *)bytes->c_str())[cursor]), &insert, sizeof(T));
        movePos(sizeof(T));
    }
};

#endif