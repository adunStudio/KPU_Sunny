#pragma once

#include <SUNNY.h>


class RoadingCircle;

class LoadingLayer2D : public Layer2D
{
public:
	Label* m_fps;

	Panel* m_panel;

	Texture2D* m_backgroundTexture;
	Sprite*    m_background;

	Texture2D*       m_circleTexture;
	vector<RoadingCircle*> m_circles;


	Sprite* m_mouse;


private:

public:
	LoadingLayer2D();
	~LoadingLayer2D();

	void OnInit(Renderer2D& renderer);
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
	void OnRender(Renderer2D& renderer);
	void OnEvent(Event& event) override;

	bool OnMouseMovedEvent  (MouseMovedEvent&   event);
	bool OnMousePressedEvent(MousePressedEvent& event);
	bool OnKeyPressedEvent  (KeyPressedEvent&   event);
};

class RoadingCircle : public Sprite
{
private:

	float m_max = 55;
	float m_min = 47;

	float m_size;

	bool first;

	bool random;

	bool up;

public:

	RoadingCircle(float x, float y, float size, directx::Texture2D* texture)
	: Sprite::Sprite(x, y, size, size, texture, PIVOT_CENTER), m_size(size), first(true), random(true), up(true)
	{
		m_max = 53 + rand() % 5;
		m_min = 45 + rand() % 5;
	}

	void Update()
	{
		if (random && rand() % 900 > 90) return;

		random = false;

		if (up)
			if (first)
				m_size += 2.5;
			else
				m_size += 0.2;
		
		else
			m_size -= 0.2;
		



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