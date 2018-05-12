#pragma once
#include"../05_GAME/shoot/BulletSystem.h"
#include <chrono>

class ShootingLayer:public Layer3D
{
public:
	ShootingLayer();
	~ShootingLayer();

	void OnInit(Renderer3D& renderer) override;
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
	void OnEvent(Event& event) override;

	bool OnKeyPressedEvent(KeyPressedEvent& event);
	bool OnKeyReleasedEvent(KeyReleasedEvent& event);
	bool OnMousePressedEvent(MousePressedEvent& event);
	bool OnMouseReleasedEvent(MouseReleasedEvent& event);

private:
	Model* m_sphere = nullptr;
	BulletSystem* m_bulletSystem2;
	BulletSystem* m_bulletSystem9;
	BulletSystem* m_bulletSystem3;
	chrono::steady_clock::time_point start;
};

