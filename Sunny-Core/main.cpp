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

	// 각 정점의 높이를 담는 배열
	std::vector<unsigned char> in(500 * 500);
	std::vector<float> height_map;

	std::ifstream inFile;

	inFile.open("resource/raw/terrain.raw", std::ios_base::binary);

	if (inFile)
	{
		std::cout << "open complete" << std::endl;

		inFile.read((char*)&in[0], (std::streamsize)in.size());

		inFile.close();
	}

	height_map.resize(500 * 500, 0);

	for (unsigned int i = 0; i < 500 * 500; ++i)
		height_map[i] = (in[i] / 255.0f);


	

	return 0;
}

