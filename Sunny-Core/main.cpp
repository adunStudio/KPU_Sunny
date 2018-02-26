#include <iostream>

#include "app/Application.h"
#include "test/TestLayer2D.h"
#include "test/TestLayer3D.h"

using namespace sunny;
using namespace graphics;
using namespace directx;

class Game : public Application
{
public:
	Game() : Application("Sunny", { 1280, 720, false, false})
	{}

	~Game()
	{}

	void Init() override
	{
		Application::Init();

		VFS::Get()->Mount("objs",     "resource/objs");
		VFS::Get()->Mount("textures", "resource/textures");
		VFS::Get()->Mount("suns",     "resource/suns");

		PushLayer(new TestLayer3D());
	//	PushLayer(new TestLayer2D());
	}
};

int main()
{
	std::cout << "Hello, Sunny Project!" << std::endl;

	Game game;

	game.Start();

    return 0;
}

