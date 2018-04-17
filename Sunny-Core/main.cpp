#include <iostream>

#include "sunny.h"
#include "app/Application.h"
#include "game/TestLayer2D.h"
#include "game/ChessLayer.h"
#include "game/TestLayer3D.h"

#include "maths/maths.h"

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

		VFS::Get()->Mount("obj",     "resource/obj");
		VFS::Get()->Mount("texture", "resource/texture");
		VFS::Get()->Mount("sun",     "resource/sun");
		VFS::Get()->Mount("raw",     "resource/raw");
		VFS::Get()->Mount("json",    "resource/json");

		PushOverlay(new TestLayer2D());
		//PushOverlay(new TestLayer3D());
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

