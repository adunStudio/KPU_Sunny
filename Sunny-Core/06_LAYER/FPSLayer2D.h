#pragma once

#include <SUNNY.h>>

class FPSLayer2D : public Layer2D
{
private:
	static FPSLayer2D* instance;

public:
	static void SetColor(vec4& color);

public:
	Label* m_fps;

public:
	FPSLayer2D();
	~FPSLayer2D();

	void OnTick() override;
};