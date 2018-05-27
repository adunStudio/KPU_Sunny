#pragma once

#include <SUNNY.h>>

class MouseLayer2D : public Layer2D
{
private:
	static MouseLayer2D* instance;

private:
	unordered_map<string, Texture2D*> m_textures;
	Sprite* m_mouse;
public:
	MouseLayer2D();
	~MouseLayer2D();

	void OnEvent(Event& event) override;

	bool OnMouseMovedEvent(MouseMovedEvent&   event);
};