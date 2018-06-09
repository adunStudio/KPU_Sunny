#pragma once

#include <SUNNY.h>


class MapGUILayer2D : public Layer2D
{
public:
	vec3& model_position = vec3();
	vec3& model_rotation = vec3();
	vec3& model_scale    = vec3();

	vec3& camera_position    = vec3();
	vec3& camera_focalPoint  = vec3();
	
private:
	bool open_transform = true;
	bool open_camera    = true;

public:
	MapGUILayer2D();
	~MapGUILayer2D();

	void OnInit(Renderer2D& renderer);
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
	void OnRender(Renderer2D& renderer);
};
