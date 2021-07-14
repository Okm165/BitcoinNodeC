#include "fstream.h"

FStream::FStream(const char* path)
{
    this->path = path;
    this->file.open(path, std::fstream::in | std::fstream::binary);
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

//read length of bytes
std::string FStream::read(uint32_t length)
{
    char* buffer = new char [length];
    this->file.read(buffer, length);
    std::string data(buffer, length);
    delete[] buffer;
    return data;
}