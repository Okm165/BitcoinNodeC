#ifndef FSTREAM_H
#define FSTREAM_H

#include "util.h"
#include <vector>


class FStream
{
    private:

    std::fstream file;

    public:

    const char* path;
    uint64_t length;

    FStream(const char* path);
    
    //get current position
    uint64_t getPos();

    //set absolute position from begining of the file 
    void setPos(uint64_t pos);

    //move position relatively to current position 
    void movePos(u_int64_t pos);

    //get length of file
    void getLength();

    //read bytes
    template<class T>
    T read()
    {
        char* buffer = new char [sizeof(T)];
        this->file.read(buffer, sizeof(T));
        T ret = *(T *)(buffer);
        delete[] buffer;
        return ret;
    }
    
    //read length of bytes
    std::string read(uint32_t length);
};

#endif