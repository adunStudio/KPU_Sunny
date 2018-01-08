#include <iostream>

#include "system/FileSystem.h"

int main()
{
    std::cout << "Hello, Sunny Project!" << std::endl;

    std::string file_path = "read_test.txt";

    bool exist =  sunny::system::FileSystem::FileExists(file_path);

    if(exist)
    {
        long size = sunny::system::FileSystem::GetFileSize(file_path);

        char* data = new char[size];

        sunny::system::FileSystem::ReadFile(file_path, data, size - 5);
        char* binary = sunny::system::FileSystem::ReadFile(file_path);
        std::string str = sunny::system::FileSystem::ReadTextFile(file_path);

        std::cout << data << std::endl;

        for(int i = 0; i < size; ++i)
            std::cout << binary[i];
        std::cout << std::endl;

        std::cout << str << std::endl;

        // write
        std::cout << "Write: " << sunny::system::FileSystem::WriteTextFile("write_test.txt", str) << std::endl;
    }

    return 0;
}
