#include <iostream>

#include "app/Application.h"
#include "test/TestLayer.h"
#include "utils/LoadImage.h"
#include "system/VFS.h"
#include "directx/Texture2D.h"
#include "directx/Shader.h"
using namespace sunny;
using namespace directx;

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
		//Texture2D* texture = Texture2D::FromFile("time.png");
		//texture->Bind();
		Shader* shader =  Shader::CreateFromFile("test", "sunny.hlsl");
		PushLayer(new TestLayer());
		PushLayer(new TestLayer());

		
	}
};


int main()
{
	std::cout << "Hello, Sunny Project!" << std::endl;
	std::cout << "Hello, Sunny Project2!" << std::endl;

	Game game;

	game.Start();

    return 0;
}

