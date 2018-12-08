#pragma once

#include <SUNNY.h>

#include "MapGUILayer2D.h"
#include "../05_GAME/graphics/Model3D.h"
#include "../05_GAME/graphics/Animation3D.h"


using namespace game;

class MapGUILayer3D : public Layer3D
{
public:
	MapGUILayer2D* m_layer2D;

	vector<Model3D*> m_mapObjects;

	Model3D* m_pickedModel = nullptr;

	Group3D* model_axis = nullptr;
	
	Entity* model_x;
	Entity* model_y;
	Entity* model_z;

	vec3 prevMouse;

	bool x_position_mode = false;
	bool y_position_mode = false;
	bool z_position_mode = false;

public:
	MapGUILayer3D();
	~MapGUILayer3D();

	void OnInit(Renderer3D& renderer) override;
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;

	void OnEvent(Event& event) override;

	bool OnKeyPressedEvent(KeyPressedEvent& event);
	bool OnKeyReleasedEvent(KeyReleasedEvent& event);
	bool OnMousePressedEvent(MousePressedEvent& event);
	bool OnMouseReleasedEvent(MouseReleasedEvent& event);
	bool OnMouseMovedEvent(MouseMovedEvent& event);
};