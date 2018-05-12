#pragma once
#include <SUNNY.h>

#include "../05_GAME/graphics/Model3D.h"
#include "../05_GAME/graphics/Animation3D.h"
#include "../05_GAME/MousePicker.h"

using namespace std;
using namespace sunny;
using namespace graphics;
using namespace directx;
using namespace events;
using namespace game;
using namespace maths;

struct Point
{
	float x;
	float y;
	float z;
};


class CBullet
{
public:
	CBullet();
	CBullet(Model* mesh, float speed, float angle, float speedAccel = 0, float angleAccel = 0);
	~CBullet();
	void Update(float elapsedTime);
	inline Point GetPosition()const { return position; }
	inline Entity* GetEntity()const { return m_entity; }
private:
	Model * m_mesh= nullptr;
	Entity* m_entity = nullptr;
	Point position = {0,0,0};
	float m_speed = 1;
	float m_angle = 0;
	float m_angleAccelation = 0;
	float m_speedAccelation = 0;
};

