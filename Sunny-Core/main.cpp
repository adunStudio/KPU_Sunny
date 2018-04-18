#include <iostream>

#include <SUNNY.h>

#include "06_LAYER/TestLayer2D.h"
#include "06_LAYER/ChessLayer.h"
#include "06_LAYER/TestLayer3D.h"

using namespace sunny;
using namespace graphics;
using namespace directx;

class Game : public Application
{
public:
	Game() : Application("½á´Ï", { 1280, 760, false, false })
	{}

	~Game()
	{}

	void Init() override
	{
		setlocale(LC_ALL, "korean");

		Application::Init();

		VFS::Get()->Mount("JSON",    "03_JSON");
		VFS::Get()->Mount("OBJ",     "04_ASSET/OBJ");
		VFS::Get()->Mount("SUN",     "04_ASSET/SUN");
		VFS::Get()->Mount("TEXTURE", "04_ASSET/TEXTURE");
		VFS::Get()->Mount("RAW",     "04_ASSET/RAW");

		//PushOverlay(new TestLayer2D());
		PushOverlay(new TestLayer3D());
		//PushOverlay(new ChessLayer());
		//PushOverlay(new TestLayer3D());
	}
};


int main()
{
	std::cout << "Hello, Sunny Project!" << std::endl;

	Game game;

	game.Start();

	return 0;
}

