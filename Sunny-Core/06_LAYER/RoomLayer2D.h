#pragma once

#include <iostream>
#include <SUNNY.h>

using namespace std;
using namespace sunny;
using namespace graphics;
using namespace events;

class RoomLayer2D : public Layer2D
{
public:
	RoomLayer2D();
	~RoomLayer2D();

	void OnInit(Renderer2D& renderer);
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
	void OnRender(Renderer2D& renderer);
	void OnEvent(Event& event) override;

	bool OnKeyPressedEvent(KeyPressedEvent& event);

	void ButtonClick();
	void Select1();
	void Select2();
	void Select3();
	void GoLeft();
	void GoRight();
public:
	int m_windowWidth, m_windowHeight;
	Panel* m_panel;
	Sprite* m_selectCharator1;
	Sprite* m_selectCharator2;
	Sprite* m_selectCharator3;

	Sprite* m_map[3];

	int m_mapSelect = 0;
};