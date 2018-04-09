#include <iostream>

#include "sunny.h"
#include "app/Application.h"
#include "game/TestLayer2D.h"
#include "game/ChessLayer.h"

#include "maths/maths.h"

using namespace sunny;
using namespace graphics;
using namespace directx;

class Game : public Application
{
public:
	Game() : Application("Sunny", { 1280, 720, false, false })
	{}

	~Game()
	{}

	void Init() override
	{
		Application::Init();

		VFS::Get()->Mount("obj",     "resource/obj");
		VFS::Get()->Mount("texture", "resource/texture");
		VFS::Get()->Mount("sun",     "resource/sun");
		VFS::Get()->Mount("raw",     "resource/raw");
		VFS::Get()->Mount("json",    "resource/json");

		PushOverlay(new TestLayer2D());
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

