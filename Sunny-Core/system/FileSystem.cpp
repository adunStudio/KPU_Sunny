//
// Created by adunstudio on 2018-01-09.
//

#include "FileSystem.h"

namespace sunny
{
    namespace system
    {
        bool FileSystem::FileExists (const std::string& path)
        {
            return GetFileSize(path) > 0;
        }

        long FileSystem::GetFileSize(const std::string& path)
        {
            std::ifstream file(path, std::ifstream::ate | std::ifstream::binary);

            long size = file.tellg();

            file.close();

            return size;  // if file is ot exists return -1
        }

        /* Read */
        unsigned char* FileSystem::ReadFile(const std::string& path)
        {
            long length = GetFileSize(path);

            if(length < 1) // TODO: Debug System
            {
                std::cout << "file not" << std::endl;
                exit(0);
            }

            char* buffer = new char[length];

            if(ReadFile(path, buffer, length))
                return (unsigned char*)(buffer);

            delete[] buffer;

            return nullptr;
        }


        bool FileSystem::ReadFile(const std::string& path, void* buffer, long length)
        {
            std::ifstream file(path, std::ifstream::in | std::ifstream::binary);

            if(!file )
                return false;

            file.seekg(0, file.end);

            if(length < 0)
                length = file.tellg();

            file.seekg(0, file.beg);
			
            file.read(static_cast<char*>(buffer), length);

            file.close();

            return true;
        }

        std::string FileSystem::ReadTextFile(const std::string& path)
        {
            std::ifstream file(path);

            if(!file )
                return std::string("");

            std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

            return str;
        }

        /* Write */
        bool FileSystem::WriteFile(const std::string& path, void* buffer, bool overwrite)
        {

            if(!overwrite && FileExists(path))
                return false;

            std::ofstream file(path.c_str(), std::ifstream::binary);

            file.write(static_cast<char*>(buffer), strlen(static_cast<char*>(buffer)));

            file.close();

            return true;
        }

        bool FileSystem::WriteTextFile(const std::string& path, const std::string& text, bool overwrite)
        {
            return WriteFile(path, (char*)&text[0], overwrite);
        }
    }
}