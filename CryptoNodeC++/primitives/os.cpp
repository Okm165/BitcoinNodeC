#include "os.h"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)

bool dirExists(const char* path)
{
    DWORD buf = GetFileAttributesA((LPCSTR)path);
    if((buf != INVALID_FILE_ATTRIBUTES) && (buf & FILE_ATTRIBUTE_DIRECTORY)){return true;}
    return false;
}

bool fileExists(const char* path)
{
    DWORD buf = GetFileAttributesA((LPCSTR)path);
    if((buf != INVALID_FILE_ATTRIBUTES) && !(buf & FILE_ATTRIBUTE_DIRECTORY)){return true;}
    return false;
}

bool mkDir(const char* path, mode_t mode){return CreateDirectoryA((LPCSTR)path, NULL);}

bool rmDir(const char* path){return RemoveDirectoryA((LPCSTR)path);}

bool clearDir(std::string& path)
{
    LPWIN32_FIND_DATAA data;
    HANDLE hdl = FindFirstFileA((LPCSTR)(path.c_str()), data);
    while(hdl != INVALID_HANDLE_VALUE)
    {
        if(strcmp(data->cFileName, ".") == 0 || strcmp(data->cFileName, "..") == 0){continue;}
        std::string sub_path = path +"/"+ data->cFileName;
        if(fileExists(sub_path.c_str())){assert(DeleteFileA((LPCSTR)sub_path.c_str()));}
        else if(dirExists(sub_path.c_str()))
        {
            assert(clearDir(sub_path));
            assert(rmDir(sub_path.c_str()));
        }
    }
    return true;
}

bool copyFile(const char* src, const char* dst){return CopyFile((LPCTSTR)src, (LPCTSTR)dst, false);}

bool copyDir(std::string& src, std::string& dst)
{
    mkDir(dst.c_str());
    LPWIN32_FIND_DATAA data;
    HANDLE hdl = FindFirstFileA((LPCSTR)(src.c_str()), data);
    while(hdl != INVALID_HANDLE_VALUE)
    {
        if(strcmp(data->cFileName, ".") == 0 || strcmp(data->cFileName, "..") == 0){continue;}
        std::string src_sub_path = src +"/"+ data->cFileName;
        std::string dst_sub_path = dst +"/"+ data->cFileName;
        
        if(fileExists(src_sub_path.c_str())){assert(copyFile(src_sub_path.c_str(), dst_sub_path.c_str()));}
        if(dirExists(src_sub_path.c_str())){assert(copyDir(src_sub_path, dst_sub_path));}
    }
    return true;
}

#elif defined(__unix__)

bool dirExists(const char* path)
{
    struct stat buf;
    stat(path, &buf);
    return S_ISDIR(buf.st_mode);
}

bool fileExists(const char* path)
{
    struct stat buf;
    stat(path, &buf);
    return S_ISREG(buf.st_mode);
}

bool mkDir(const char* path, mode_t mode){return mkdir(path, mode);}

bool rmDir(const char* path){return rmdir(path);}

bool clearDir(std::string& path)
{
    DIR* dir;
    dirent* dirfile;
    dir = opendir(path.c_str());
    while((dirfile = readdir(dir)) != NULL)
    {
        if(strcmp(dirfile->d_name, ".") == 0 || strcmp(dirfile->d_name, "..") == 0){continue;}
        std::string sub_path = path +"/"+ dirfile->d_name;
        if(fileExists(sub_path.c_str())){assert(!remove(sub_path.c_str()));}
        else if(dirExists(sub_path.c_str()))
        {
            assert(clearDir(sub_path));
            assert(!rmDir(sub_path.c_str()));
        }
    }
    return true;
}

bool copyFile(const char* src, const char* dst)
{
    int source = open(src, O_RDONLY, 0);
    int dest = open(dst, O_WRONLY | O_CREAT, 0777);

    struct stat buf;
    fstat(source, &buf);
    
    sendfile(dest, source, 0, buf.st_size);
    close(source);
    close(dest);

    return true;
}

bool copyDir(std::string& src, std::string& dst)
{
    mkDir(dst.c_str());
    DIR* dir;
    dirent* dirfile;
    dir = opendir(src.c_str());
    while((dirfile = readdir(dir)) != NULL)
    {
        if(strcmp(dirfile->d_name, ".") == 0 || strcmp(dirfile->d_name, "..") == 0){continue;}
        std::string src_sub_path = src + "/" + dirfile->d_name;
        std::string dst_sub_path = dst + "/" + dirfile->d_name;

        if(fileExists(src_sub_path.c_str())){assert(copyFile(src_sub_path.c_str(), dst_sub_path.c_str()));}
        else if(dirExists(src_sub_path.c_str())){assert(copyDir(src_sub_path, dst_sub_path));}
    }
    return true;
}

#endif









