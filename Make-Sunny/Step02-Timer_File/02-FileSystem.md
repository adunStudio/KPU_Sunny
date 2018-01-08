# FileSystem

> STL 강의를 수강했다는 가정하에 fstream 표준 라이브러리를 사용합니다.

리소스 데이터 파일을 읽거나 게임 진행 내용을 Save / Load(Write / Read) 해야하는 경우가 있습니다.

---

Sunny Engine은 C++ STL fstream 라이브러리를 이용한 데이터를 읽고 쓰는 FileSystem 클래스를 제공합니다.

```cpp
class FileSystem
{
private:
    FileSystem() = default;

public:
    // static 키워드를 사용한 전역 접근

    // 파일의 존재 유뮤를 반환한다.
    static bool FileExists (const std::string& path);
    // 파일의 사이즈(데이터의 길이)를 반환한다.
    static long GetFileSize(const std::string& path);

    // 파일 일기
    static char* ReadFile(const std::string& path);
    static bool ReadFile(const std::string& path, char* buffer, long length = -1);
    static std::string ReadTextFile(const std::string& path);

    // 파일 쓰기
    static bool WriteFile(const std::string& path, char* buffer, bool overwrite = true);
    static bool WriteTextFile(const std::string& path, const std::string& text, bool overwrite = true);
};
```

---

간단한 사용 예시를 보여드리겠습니다.

```cpp
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
        std::cout << "Write: "<< sunny::system::FileSystem::WriteTextFile("write_test.txt", str) << std::endl;
    }

    return 0;
}

Hello, Sunny Project!
This file is
This file is Test
This file is Test
Write: 1
```
---

### Commit
[Step 02. Timer & File - FileSystem]()
