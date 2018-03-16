#include <iostream>

#include "app/Application.h"
#include "game/TestLayer2D.h"
#include "game/ChessLayer.h"

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

		VFS::Get()->Mount("objs",     "resource/objs");
		VFS::Get()->Mount("textures", "resource/textures");
		VFS::Get()->Mount("suns",     "resource/suns");

		PushOverlay(new TestLayer2D());
		//PushOverlay(new ChessLayer());
	}
};

#include <thread>

int s_data;
bool flag = false;

void thread_recv()
{
	while (false == flag)
		std::cout << "I receive [ " << s_data << " ] " << std::endl;
}

void thread_send()
{
	s_data = 999;
	flag = true;
	std::cout << "I havesent [ " << s_data << " ]" << std::endl;
}


int main()
{
	std::cout << "Hello, Sunny Project!" << std::endl;

	Game game;

	game.Start();

	thread rt{ thread_recv };
	thread st{ thread_send };
	rt.join();
	st.join();



	return 0;
}

