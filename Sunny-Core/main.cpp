#include <iostream>

#include <SUNNY.h>

#include "05_GAME/assets/AssetLoader.h"

#include "06_LAYER/FPSLayer2D.h"
#include "06_LAYER/LoadingLayer2D.h"

#include "06_LAYER/RoomLayer2D.h"
#include "06_LAYER/TestLayer2D.h"


class Game : public Application
{
public:
	Game() : Application("sunny", { 1600, 900, false, false })
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
		
		//Server::Connect("221.142.221.186", "7711");


		if (Server::IsConnected())
			cout << "서버 연결 완료" << endl;
		else
			cout << "서버 연결 실패" << endl;

		game::AssetLoader::LoadModelData("/JSON/MODEL/Trees.json");
		game::AssetLoader::LoadModelData("/JSON/MODEL/LowPolyNatures.json");
	//	game::AssetLoader::LoadModel("/JSON/CHARACTER/Characters.json");
		//game::AssetLoader::LoadTexture("/JSON/TEXTURE/Rooms.json");

		//Input::GetInputManager()->SetMouseCursor(SUNNY_NO_CURSOR);

		//PushOverlay(new RoomLayer2D());
		PushOverlay(new FPSLayer2D());
		FPSLayer2D::SetColor(RGBA(0.f, 0.f, 0.f, 0.7f));

		PushLayer(new LoadingLayer2D());
		
	}
};


int main()
{
	std::cout << "Hello, Sunny Project!" << std::endl;

	Game game;

	game.Start();

	return 0;
}



