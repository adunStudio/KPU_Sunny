#pragma once

#include <SUNNY.h>

class MapGUILayer2D : public Layer2D
{
public:
	MapGUILayer2D();
	~MapGUILayer2D();

	void OnInit(Renderer2D& renderer);
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
	void OnRender(Renderer2D& renderer);
};
