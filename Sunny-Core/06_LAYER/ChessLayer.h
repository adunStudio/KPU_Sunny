#pragma once

#include <iostream>
#include <SUNNY.h>

using namespace std;
using namespace sunny;
using namespace graphics;
using namespace events;

#define KNIGHT 1

class ChessLayer : public Layer2D
{
private:
	Sprite* m_knight;
	int m_board[8][8];

public:
	ChessLayer();
	~ChessLayer();

	void OnInit(Renderer2D& renderer);
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
	void OnRender(Renderer2D& renderer);
	void OnEvent(Event& event) override;

	bool OnKeyPressedEvent(KeyPressedEvent& event);
};