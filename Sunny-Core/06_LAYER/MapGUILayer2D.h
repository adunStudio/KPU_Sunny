#pragma once

#include <SUNNY.h>
#include "../05_GAME/graphics/Model3D.h"

using namespace game;

class MapGUILayer2D : public Layer2D
{
public:
	Model3D* m_pickedModel = nullptr;

	vec3& model_position = vec3(0);
	vec3& model_rotation = vec3(0);
	vec3& model_scale    = vec3(0);

	vec3& camera_position    = vec3(0);
	vec3& camera_focalPoint  = vec3(0);
	
private:
	bool open_transform = true;
	bool open_terrain   = true;
	bool open_camera    = true;

public:
	MapGUILayer2D();
	~MapGUILayer2D();

	void OnInit(Renderer2D& renderer);
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
	void OnRender(Renderer2D& renderer);

	void SetPickedModel(Model3D* model);
};
