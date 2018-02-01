#include <iostream>

#include "app/Application.h"
#include "system/VFS.h"

int main()
{
	//std::cout << "Hello, Sunny Project!" << std::endl;

	//sunny::Application game("Test", {800, 600, false, false});

	//game.Start();

//	std::string a = "dd// dd ";

	//std::cout << sunny::utils::FindToken(a, "dd");// << std::endl;

	//const char* ad = "SDfsaaaaaaaaaaaaaaaaaad";

	//std::string b = std::string(ad, 5);

	//std::cout << b;// << std::endl;
	sunny::VFS::Init();

	const std::string path = "sunny.h";

	std::string a = sunny::VFS::Get()->ReadTextFile(path);
	unsigned char* a2 = sunny::VFS::Get()->ReadFile(path);
	std::cout << a << std::endl;
	std::cout << a2 << std::endl;

	if (! sunny::VFS::Get()->WriteFile("aa.h", a2) )
	{
		std::cout << "쓰기 실패1" << std::endl;
	}
	else
	{
		std::cout << "쓰기 성공1" << std::endl;
	}

	if (!sunny::VFS::Get()->WriteTextFile("aa2.h", a))
	{
		std::cout << "쓰기 실패2" << std::endl;
	}
	else
	{
		std::cout << "쓰기 성공2" << std::endl;
	}

    return 0;
}