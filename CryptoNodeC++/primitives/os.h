#ifndef OS_H
#define OS_H

#include <string>
#include <cassert>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

inline bool dirExists(const char* path);
inline bool fileExists(const char* path);
inline bool mkDir(const char* path, mode_t mode = 0777);
inline bool rmDir(const char* path);

bool clearDir(std::string& path);

bool copyFile(const char* src, const char* dst);

bool copyDir(std::string& src, std::string& dst);

#endif