#include "RoomLayer2D.h"



RoomLayer2D::RoomLayer2D()
	: Layer2D(maths::mat4::Orthographic(0.0f, Application::GetApplication().GetWindowWidth(), 0.0f, Application::GetApplication().GetWindowHeight(), -1.0f, 1.0f))
{
}


RoomLayer2D::~RoomLayer2D()
{
}

int H(int num)
{
	return 900 - num;
}




void RoomLayer2D::OnInit(Renderer2D & renderer)
{
	m_windowWidth = Application::GetApplication().GetWindowWidth();
	m_windowHeight = Application::GetApplication().GetWindowHeight();

	m_panel = new Panel();

	Sprite* background = new Sprite(0, 0, 1600, 900, new Texture2D("/TEXTURE/Room/room_background.jpg"));

	Sprite* face14 = new Sprite(350, 640, 150, 150, new Texture2D("/TEXTURE/Room/14_face.png"));
	Sprite* face14_pressed = new Sprite(350, 640, 150, 150, new Texture2D("/TEXTURE/Room/14_face_pressed.png"));
	Sprite* face15 = new Sprite(350, 470, 150, 150, new Texture2D("/TEXTURE/Room/15_face.png"));
	Sprite* face15_pressed = new Sprite(350, 470, 150, 150, new Texture2D("/TEXTURE/Room/15_face_pressed.png"));
	Sprite* face20 = new Sprite(350, 300, 150, 150, new Texture2D("/TEXTURE/Room/20_face.png"));
	Sprite* face20_pressed = new Sprite(350, 300, 150, 150, new Texture2D("/TEXTURE/Room/20_face_pressed.png"));

	Sprite* leftarrow = new Sprite(850, 245, 50, 50, new Texture2D("/TEXTURE/Room/leftArrow.png"));
	Sprite* leftarrow_pressed = new Sprite(850, 245, 50, 50, new Texture2D("/TEXTURE/Room/leftArrow_pressed.png"));
	Sprite* rightarrow = new Sprite(1190, 245, 50, 50, new Texture2D("/TEXTURE/Room/rightArrow.png"));
	Sprite* rightarrow_pressed = new Sprite(1190, 245, 50, 50, new Texture2D("/TEXTURE/Room/rightArrow_pressed.png"));
	//Sprite* face20 = new Sprite(350, 300, 150, 150, new Texture2D("/TEXTURE/Room/20_face.png"));



	Sprite* team1 = new Sprite(840, 590, 200, 200, new Texture2D("/TEXTURE/Room/15_face.png"));
	Sprite* team2 = new Sprite(1050, 590, 200, 200, new Texture2D("/TEXTURE/Room/20_face.png"));


	m_selectCharator1 = new Sprite(520, 490, 300, 300, new Texture2D("/TEXTURE/Room/14_face.png"));
	m_selectCharator2 = new Sprite(520, 490, 300, 300, new Texture2D("/TEXTURE/Room/15_face.png"));
	m_selectCharator3 = new Sprite(520, 490, 300, 300, new Texture2D("/TEXTURE/Room/20_face.png"));

	//Sprite* map = new Sprite(840, 160, 410, 410, new Texture2D("/TEXTURE/Room/stage1.png"));
	m_map[0] = new Sprite(840, 160, 410, 230, new Texture2D("/TEXTURE/Room/plain.jpg"));
	m_map[1] = new Sprite(840, 160, 410, 230, new Texture2D("/TEXTURE/Room/ice.jpg"));
	m_map[2] = new Sprite(840, 160, 410, 230, new Texture2D("/TEXTURE/Room/desert.jpg"));

	Sprite* layout = new Sprite(320, 90, 960, 720, new Texture2D("/TEXTURE/Room/layout.png"));


	Sprite* start = new Sprite(350, 160, 470, 120, new Texture2D("/TEXTURE/Room/startButton.png"));
	Sprite* start_pressed = new Sprite(350, 160, 470, 120, new Texture2D("/TEXTURE/Room/startButton_pressed.png"));

	Sprite* master = new Sprite(865, 600, 150, 50, new Texture2D("/TEXTURE/Room/master.png"));

	Sprite* ready = new Sprite(1075, 600, 150, 50, new Texture2D("/TEXTURE/Room/readyButton.png"));
	Sprite* ready_pressed = new Sprite(1075, 600, 150, 50, new Texture2D("/TEXTURE/Room/readyButton_pressed.png"));


	Sprite* chat = new Sprite(350, 110, 900, 30, new Texture2D("/TEXTURE/Room/chat.png"));
	/*Sprite* text = new Sprite(350, 160, 470, 120, new Texture2D("/TEXTURE/Room/text.png"));*/
	//Sprite* face15 = new Sprite(270, 200,250,250, new Texture2D("/TEXTURE/Room/15_face.png"));
	//Sprite* face15_pressed = new Sprite(270, 200, 260, 260, new Texture2D("/TEXTURE/Room/15_face_pressed.png"));
	//Sprite* face20 = new Sprite(530, 200, 250, 250, new Texture2D("/TEXTURE/Room/20_face.png"));
	//Sprite* face20_pressed = new Sprite(520, 180, 260, 260, new Texture2D("/TEXTURE/Room/20_face_pressed.png"));
	//Sprite* start = new Sprite(190, 50, 430, 100, new Texture2D("/TEXTURE/Room/startButton.png"));

	Button* faceButton1 = new Button(face14, face14_pressed, LAMBDA(RoomLayer2D::Select1));
	Button* faceButton2 = new Button(face15, face15_pressed, LAMBDA(RoomLayer2D::Select2));
	Button* faceButton3 = new Button(face20, face20_pressed, LAMBDA(RoomLayer2D::Select3));


	Button* startButton = new Button(start, start_pressed, LAMBDA(RoomLayer2D::ButtonClick));
	Button* readyButton = new Button(ready, ready_pressed, LAMBDA(RoomLayer2D::ButtonClick));

	Button* rightArrowButton = new Button(rightarrow, rightarrow_pressed, LAMBDA(RoomLayer2D::GoRight));
	Button* leftArrowButton = new Button(leftarrow, leftarrow_pressed, LAMBDA(RoomLayer2D::GoLeft));
	//Button* faceButton2 = new Button(face15, face15_pressed, LAMBDA(RoomLayer2D::ButtonClick));
	//Button* faceButton3 = new Button(face20, face20_pressed, LAMBDA(RoomLayer2D::ButtonClick));
	//Button* startButton = new Button(start, LAMBDA(RoomLayer2D::ButtonClick));

	Add(background);
	Add(layout);

	m_panel->Add(faceButton1);
	m_panel->Add(faceButton2);
	m_panel->Add(faceButton3);

	Add(m_selectCharator1);
	Add(m_selectCharator2);
	Add(m_selectCharator3);

	Add(m_map[0]);
	Add(m_map[1]);
	Add(m_map[2]);

	Add(team1);
	Add(team2);


	//Add(text);
	Add(chat);
	Add(master);

	m_panel->Add(startButton);
	m_panel->Add(readyButton);
	m_panel->Add(rightArrowButton);
	m_panel->Add(leftArrowButton);

	//m_panel->Add(faceButton2);
	//m_panel->Add(faceButton3);

	//m_panel->Add(startButton);
}

void RoomLayer2D::OnTick()
{
}

void RoomLayer2D::OnUpdate(const utils::Timestep & ts)
{

}

void RoomLayer2D::OnRender(Renderer2D & renderer)
{
}

void RoomLayer2D::OnEvent(Event & event)
{
}

bool RoomLayer2D::OnKeyPressedEvent(KeyPressedEvent & event)
{
	return false;
}

void RoomLayer2D::ButtonClick()
{


}

void RoomLayer2D::Select1()
{
	m_selectCharator1->SetVisible(true);
	m_selectCharator2->SetVisible(false);
	m_selectCharator3->SetVisible(false);
}

void RoomLayer2D::Select2()
{
	m_selectCharator2->SetVisible(true);
	m_selectCharator1->SetVisible(false);
	m_selectCharator3->SetVisible(false);
}

void RoomLayer2D::Select3()
{
	m_selectCharator3->SetVisible(true);
	m_selectCharator1->SetVisible(false);
	m_selectCharator2->SetVisible(false);
}

void RoomLayer2D::GoLeft()
{
	m_map[0]->SetVisible(false);
	m_map[1]->SetVisible(false);
	m_map[2]->SetVisible(false);

	m_mapSelect -= 1;
	if (m_mapSelect < 0)
	{
		m_mapSelect = 2;
	}
	m_map[m_mapSelect]->SetVisible(true);
}

void RoomLayer2D::GoRight()
{
	m_map[0]->SetVisible(false);
	m_map[1]->SetVisible(false);
	m_map[2]->SetVisible(false);

	m_mapSelect += 1;
	m_mapSelect = m_mapSelect % 3;
	m_map[m_mapSelect]->SetVisible(true);
}