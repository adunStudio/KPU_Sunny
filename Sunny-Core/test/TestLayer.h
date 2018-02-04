#pragma once

#include <iostream>
#include "../graphics/layers/Layer.h"
#include "../app/Application.h"

using namespace std;
using namespace sunny;
using namespace graphics;

class TestLayer : public Layer
{
public:
	TestLayer();
	~TestLayer();

	void Init() override;
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
	void OnRender() override;
};