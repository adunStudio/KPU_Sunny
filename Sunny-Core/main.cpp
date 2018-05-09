#include <iostream>

#include <SUNNY.h>

#include "05_GAME/assets/AssetLoader.h"

#include "06_LAYER/TestLayer2D.h"
#include "06_LAYER/ChessLayer.h"
#include "06_LAYER/TestLayer3D.h"




class Game : public Application
{
public:
	Game() : Application("sunny", { 1600, 900, false, false })
	//	Game() : Application("sunny", { 600, 600, false, true })
	{}

	~Game()
	{}

	void Init() override
	{
		Application::Init();

		VFS::Get()->Mount("HLSL",    "02_HLSL");
		VFS::Get()->Mount("JSON",    "03_JSON");
		VFS::Get()->Mount("CUBE",    "04_ASSET/CUBE");
		VFS::Get()->Mount("OBJ",     "04_ASSET/OBJ");
		VFS::Get()->Mount("SUN",     "04_ASSET/SUN");
		VFS::Get()->Mount("TEXTURE", "04_ASSET/TEXTURE");
		VFS::Get()->Mount("RAW",     "04_ASSET/RAW");
		
		Server::Connect("127.0.0.1", "7711");


		if (Server::IsConnected())
			cout << "서버 연결 완료" << endl;
		else
			cout << "서버 연결 실패" << endl;





		game::AssetLoader::LoadModel("/JSON/MODEL/Trees.json");
		game::AssetLoader::LoadModel("/JSON/MODEL/LowPolyNatures.json");

	
		//PushOverlay(new TestLayer2D());
		//PushOverlay(new TestLayer3D());
		PushOverlay(new ChessLayer());
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



