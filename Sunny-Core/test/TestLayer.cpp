#include "TestLayer.h"

TestLayer::TestLayer()
{
	cout << "TestLayer::TestLayer()" << endl;
}

TestLayer::~TestLayer()
{

}

void TestLayer::Init()
{
	cout << "TestLayer::Init()" << endl;
}

void TestLayer::OnTick() 
{
	cout << "TestLayer::OnTick()" << endl;
	std::cout << "UPS: " << Application::GetApplication().GetUPS() << std::endl;
	std::cout << "FPS: " << Application::GetApplication().GetFPS() << std::endl;
}

void TestLayer::OnUpdate(const utils::Timestep& ts)
{
	//cout << "TestLayer::OnUpdate()" << endl;
}

void TestLayer::OnRender()
{
	//cout << "TestLayer::OnRender()" << endl;
}