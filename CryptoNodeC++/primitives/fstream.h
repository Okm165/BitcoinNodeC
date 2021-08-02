/*
FStream functionalty --> filestream
------------------------------------------------------------------------------------------
In order to use FStream create object FStream
------------------------------------------------------------------------------------------
FStream(const char* path, FStreamOpCodes opcode)
filestream serves file read/write functionalities,
<path> is path to file,
<opcode> is mode with which the file will be opened,
avilable opcodes: READONLY, WRITEONLY, READWRITE
------------------------------------------------------------------------------------------
*/

#ifndef FSTREAM_H
#define FSTREAM_H

#include "util.h"
#include <vector>

enum FStreamOpCodes
{
    READONLY,
    WRITEONLY,
    READWRITE
};

class FStream
{
    protected:

    std::fstream file;
    const char* path;
    FStreamOpCodes opcode;

    public:

    uint64_t length;

    FStream(const char* path, FStreamOpCodes opcode);
    
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
    std::string read(uint64_t length);

    //write data to file has to have WRITE permissions
    void write(std::string& data);

    void close();
};

#endif