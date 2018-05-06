#include <iostream>

#include <SUNNY.h>

#include "05_GAME/assets/AssetLoader.h"

#include "06_LAYER/TestLayer2D.h"
#include "06_LAYER/ChessLayer.h"
#include "06_LAYER/TestLayer3D.h"




class Game : public Application
{
public:
	Game() : Application("sunny", { 1600, 900, false, true })
	{}

	~Game()
	{}

	void Init() override
	{
		//setlocale(LC_ALL, "korean");

		Application::Init();

		VFS::Get()->Mount("HLSL",    "02_HLSL");
		VFS::Get()->Mount("JSON",    "03_JSON");
		VFS::Get()->Mount("CUBE",    "04_ASSET/CUBE");
		VFS::Get()->Mount("OBJ",     "04_ASSET/OBJ");
		VFS::Get()->Mount("SUN",     "04_ASSET/SUN");
		VFS::Get()->Mount("TEXTURE", "04_ASSET/TEXTURE");
		VFS::Get()->Mount("RAW",     "04_ASSET/RAW");
		
		//SetServer(new ChessServer("127.0.0.1", "7711"));
		//new Server("127.0.0.1", "7711");

		Server::Connect("127.0.0.1", "7711");

		//game::AssetLoader::LoadModel("/JSON/MODEL/Trees.json");
		//game::AssetLoader::LoadModel("/JSON/MODEL/LowPolyNatures.json");

	
		std::cout << Server::IsConnected() << std::endl;

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



