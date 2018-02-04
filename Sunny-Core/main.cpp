#include <iostream>

#include "app/Application.h"
#include "test/TestLayer.h"

using namespace sunny;

class Game : public Application
{
public:
	Game() : Application("Test", { 800, 600, false, false })
	{}

	~Game()
	{}

	void Init() override
	{
		Application::Init();

		PushLayer(new TestLayer());
		PushLayer(new TestLayer());
	}
};


int main()
{
	std::cout << "Hello, Sunny Project!" << std::endl;

	Game game;

	game.Start();

    return 0;
}

