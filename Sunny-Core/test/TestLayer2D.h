#pragma once

#include <iostream>
#include "../graphics/layers/Layer2D.h"
#include "../app/Application.h"

using namespace std;
using namespace sunny;
using namespace graphics;

class TestLayer2D : public Layer2D
{
public:
	TestLayer2D();
	~TestLayer2D();

	void OnInit(Renderer2D& renderer);
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
	void OnRender(Renderer2D& renderer);
};