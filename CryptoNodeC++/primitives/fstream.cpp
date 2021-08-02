#include "fstream.h"

FStream::FStream(const char* path, FStreamOpCodes opcode)
{
    this->path = path;
    this->opcode = opcode;
    if(opcode == READONLY){this->file.open(path, std::fstream::in | std::fstream::binary);}
    else if(opcode == WRITEONLY){this->file.open(path, std::fstream::out | std::fstream::binary);}
    else if(opcode == READWRITE){this->file.open(path, std::fstream::in | std::fstream::out | std::fstream::binary);}

    //check if file opens correctly
    assert(this->file.is_open());
    this->getLength();
    this->setPos(0);
}

uint64_t FStream::getPos(){return this->file.tellg();}

void FStream::setPos(uint64_t pos){this->file.seekg(pos, std::fstream::ios_base::beg);}

void FStream::movePos(uint64_t pos){this->file.seekg(pos, std::fstream::ios_base::cur);}

void FStream::getLength()
{
    this->file.seekg(0, std::fstream::ios_base::end);
    this->length = file.tellg();
    this->setPos(0);
}

std::string FStream::read(uint64_t length)
{
    char* buffer = new char [length];
    this->file.read(buffer, length);
    std::string data(buffer, length);
    delete[] buffer;
    return data;
}

void FStream::write(std::string& data){this->file << data;}

void FStream::close(){this->file.close();}