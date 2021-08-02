/*
Os functionalty --> operating system functionalities
------------------------------------------------------------------------------------------
*/

#ifndef OS_H
#define OS_H

#include <string>
#include <cassert>
#include <unistd.h>
#include <string.h>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)

#include <winbase.h>

#elif defined(__unix__)

#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#endif

bool dirExists(const char* path);
bool fileExists(const char* path);
bool mkDir(const char* path, mode_t mode = 0777);
bool rmDir(const char* path);

bool clearDir(std::string& path);

bool copyFile(const char* src, const char* dst);

bool copyDir(std::string& src, std::string& dst);

#endif