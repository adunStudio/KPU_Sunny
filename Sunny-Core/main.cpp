#include <iostream>

#include "app/Application.h"
#include "game/TestLayer2D.h"
#include "game/ChessLayer.h"
#include "game/TestLayer3D.h"

#include "maths//maths.h"

using namespace sunny;
using namespace graphics;
using namespace directx;

class Game : public Application
{
public:
	Game() : Application("Sunny", { 1280, 720, false, false })
	{}

	~Game()
	{}

	void Init() override
	{
		Application::Init();

		VFS::Get()->Mount("objs",     "resource/objs");
		VFS::Get()->Mount("textures", "resource/textures");
		VFS::Get()->Mount("suns",     "resource/suns");

		PushOverlay(new TestLayer2D());
		//PushOverlay(new ChessLayer());
		//PushOverlay(new TestLayer3D());
	}
};


int main()
{
	std::cout << "Hello, Sunny Project!" << std::endl;

	//Game game;

	//game.Start();

	using namespace maths;

	vec3 Transform;
	mat4 Rotation;
	vec3 Scale;

	mat4 m = mat4::Identity() * mat4::Translate(vec3(10, 20, 30)) * mat4::Rotate(45, vec3(0, 1, 0)) *  mat4::Scale(vec3(1, 2.3, 3.2));

	Transform = vec3(m.rows[0].w, m.rows[1].w, m.rows[2].w);

	vec3 s_x = vec3(m.rows[0].x, m.rows[1].x, m.rows[2].x);
	vec3 s_y = vec3(m.rows[0].y, m.rows[1].y, m.rows[2].y);
	vec3 s_z = vec3(m.rows[0].z, m.rows[1].z, m.rows[2].z);

	float Sx = s_x.Magnitude();
	float Sy = s_y.Magnitude();
	float Sz = s_z.Magnitude();

	std::cout << m.ToString() << std::endl;


	std::cout << "Transform : " << Transform << std::endl;
	
	std::cout << "Sx : " << Sx << std::endl;
	std::cout << "Sy : " << Sy << std::endl;
	std::cout << "Sz : " << Sz << std::endl;
	
	Scale = vec3(Sx, Sy, Sz);

	std::cout << "Scale : " << Scale << std::endl;

	vec4 row0 = vec4(m.rows[0].x / Sx, m.rows[1].x / Sx, m.rows[2].x / Sx, 0);
	vec4 row1 = vec4(m.rows[0].y / Sy, m.rows[1].y / Sy, m.rows[2].y / Sx, 0);
	vec4 row2 = vec4(m.rows[0].z / Sz, m.rows[1].z / Sz, m.rows[2].z / Sz, 0);
	vec4 row3 = vec4(               0,                0,                0, 1);

	Rotation = mat4(row0, row1, row2, row3);

	std::cout << "Rotation : " << Rotation.ToString() << std::endl;

	mat4 r =  mat4::Rotate(45, vec3(0, 1, 0));
	
	std::cout << "R: " << r.ToString() << std::endl;

	return 0;
}

