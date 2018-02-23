#include "TestLayer2D.h"

TestLayer2D::TestLayer2D()
: Layer2D(maths::mat4::Orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f))
{
	cout << "TestLayer::TestLayer()" << endl;
}

TestLayer2D::~TestLayer2D()
{

}

void TestLayer2D::OnInit(Renderer2D& renderer)
{
	cout << "TestLayer::OnInit()" << endl;
	Add(new Sprite(0.0f, 0.0f, 0.9f,0.9f, RGBA(1, 0, 0, 1)));
	Add(new Sprite(-5, -5, 3, 3, directx::Texture2D::CreateFromFIle("time.png")));
}

void TestLayer2D::OnTick()
{
	cout << "TestLayer::OnTick()" << endl;
	std::cout << "UPS: " << Application::GetApplication().GetUPS() << std::endl;
	std::cout << "FPS: " << Application::GetApplication().GetFPS() << std::endl;
}

void TestLayer2D::OnUpdate(const utils::Timestep& ts)
{
	//cout << "TestLayer::OnUpdate()" << endl;
}

void TestLayer2D::OnRender(Renderer2D& renderer)
{
	//cout << "TestLayer::OnRender()" << endl;
	renderer.DrawLine(0, 0, 3, 3, maths::vec4(0, 0, 1, 0.3), 0.1);
	renderer.DrawRect(0, 0, 3, 3, maths::vec4(0, 0, 1, 0.3));
	renderer.FillRect(0, 0, 3, 3, maths::vec4(0, 0, 1, 0.3));
}