#pragma once

#include <SUNNY.h>>

class MouseLayer2D : public Layer2D
{
private:
	static MouseLayer2D* instance;

public:
	static void SetCursor(const std::string& name);

private:
	unordered_map<string, Texture2D*> m_textures;
	Sprite* m_mouse;
	int m_size;
	bool m_center;

private:
	void SetCursorInternal(const std::string& name);

public:
	MouseLayer2D();
	~MouseLayer2D();

	void OnEvent(Event& event) override;

	bool OnMouseMovedEvent(MouseMovedEvent&   event);
};