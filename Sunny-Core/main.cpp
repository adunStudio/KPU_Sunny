#include <iostream>

#include "app/Application.h"
#include "utils/LoadImage.h"
#include "system/VFS.h"
#include "directx/Texture2D.h"
#include "directx/Shader.h"
#include "directx/Renderer.h"
#include "graphics/shaders/ShaderFactory.h";
#include "graphics/shaders/ShaderManager.h";
#include "graphics/renderables/Renderable2D.h"
#include "graphics/renderers/Renderer2D.h"
#include "graphics/Model.h"


#include "test/TestLayer2D.h"
#include "test/TestLayer3D.h"

using namespace sunny;
using namespace graphics;
using namespace directx;

class Game : public Application
{
public:
	Game() : Application("Sunny", { 800, 600, false, true })
	{}

	~Game()
	{}

	void Init() override
	{
		Application::Init();
		//Texture2D* texture = Texture2D::FromFile("time.png");
		//texture->Bind();
		//Shader* shader = sunny::graphics::ShaderFactory::DefaultShader();
		
		//Shader* shader = Shader::CreateFromFile("default", "sunny.hlsl");
		//Material* m = new Material(shader);
		//MaterialInstance* ml = new MaterialInstance(m);
	

		PushLayer(new TestLayer3D());
		//PushLayer(new TestLayer2D());
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

