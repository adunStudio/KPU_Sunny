#pragma once

#include <SUNNY.h>
#include <thread>
#include <future>
#include <unordered_map>
#include "../05_GAME/ui/Loadingbar.h"
#include "../05_GAME/assets/AssetLoader.h"
#include "../06_LAYER/RoomLayer2D.h"
#include "../06_LAYER/MapGUILayer3D.h"
#include "../06_LAYER/ParticleLayer3D.h"

using namespace game;

class RoadingCircle;

class LoadingLayer2D : public Layer2D
{
public:
	Panel* m_panel;

	Texture2D* m_backgroundTexture;
	Sprite*    m_background;

	Texture2D*       m_circleTexture;
	vector<RoadingCircle*> m_circles;

	Texture2D* m_logoTexture;
	Sprite*    m_logo;

	Label* m_state;
	Loadingbar* m_loadingBar;
	Label* m_ing;

	int m_tickCount = 0;
	int m_updateCount = 0;

	thread* m_thread;

	bool m_loadingCompleted;

private:

public:
	LoadingLayer2D();
	~LoadingLayer2D();

	void OnInit(Renderer2D& renderer);
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
	void OnRender(Renderer2D& renderer);
	void OnEvent(Event& event) override;

	bool OnMousePressedEvent(MousePressedEvent& event);
	bool OnKeyPressedEvent  (KeyPressedEvent&   event);

	void OnLoadingCompleted(float f);
	void GoToRoom();
};


class RoadingCircle : public Sprite
{
private:

	float m_max = 55;
	float m_min = 47;

	float m_size;

	bool first;


	bool up;

public:
	float start = 29;
	float time  = 0;

	RoadingCircle(float x, float y, float size, directx::Texture2D* texture)
	: Sprite::Sprite(x, y, size, size, texture, PIVOT_CENTER), m_size(size), first(true), up(true)
	{
		m_max = 53 + rand() % 5;
		m_min = 45 + rand() % 5;
	}

	void Update()
	{
		time++;

		if (time < start) return;

		if (up)
			if (first)
				m_size += 1.5;
			else
				m_size += 0.17;
		
		else
			m_size -= 0.15;
		



		if (m_size >= m_max)
		{
			up = false;
			first = false;
		}
		
		if (m_size <= m_min && first == false)
			up = true;
			
		
		SetSize(vec2(m_size, m_size));
	}
};