//
// Created by adunstudio on 2018-01-09.
//

#pragma once

#include "../include.h"
#include "VFS.h"

namespace sunny
{
    namespace system
    {
        class FileSystem
        {
        private:
            FileSystem() = default;

        public:
            // 파일의 존재 유뮤를 반환한다.
            static bool FileExists (const std::string& path);
            // 파일의 사이즈(데이터의 길이)를 반환한다.
            static long GetFileSize(const std::string& path);

            // 파일 일기
            static unsigned char* ReadFile(const std::string& path);
            static bool ReadFile(const std::string& path, void* buffer, long length = -1);
            static std::string ReadTextFile(const std::string& path);

            // 파일 쓰기
            static bool WriteFile(const std::string& path, void* buffer, bool overwrite = true);
            static bool WriteTextFile(const std::string& path, const std::string& text, bool overwrite = true);
        };
    }
}