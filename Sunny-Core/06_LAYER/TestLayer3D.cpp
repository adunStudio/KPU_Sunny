#include "TestLayer3D.h"

template <typename T> string tostr(const T& t) {
	ostringstream os;
	os << t;
	return os.str();
}



TestLayer3D::TestLayer3D()
{

}

TestLayer3D::~TestLayer3D()
{

}

void TestLayer3D::OnInit(Renderer3D& renderer)
{

	m_sphere = new Model("/OBJ/sphere.obj");
	Entity* e = new Entity(m_sphere->GetMesh(), RGBA(1.0, 0, 0, 1.0));
	Add(e);

	m_mousePicker = new MousePicker(GetCamera());

	LightSetup* lights = new LightSetup();
	Light* light = new Light(vec3(0.3, 0.3, 0.3), 2, vec4(1.f, 1.f, 1.f, 1.f));

	lights->Add(light);

	PushLightSetup(lights);


	Entity* xAxis = new Entity(MeshFactory::CreateXAxis(), RGBA(1, 0, 0, 1), mat4::Identity() * mat4::Scale(vec3(100, 100, 100)));
	Entity* yAxis = new Entity(MeshFactory::CreateYAxis(), RGBA(0, 1, 0, 1), mat4::Identity() * mat4::Scale(vec3(100, 100, 100)));
	Entity* zAxis = new Entity(MeshFactory::CreateZAxis(), RGBA(0, 0, 1, 1), mat4::Identity() * mat4::Scale(vec3(100, 100, 100)));
	AddStatic(xAxis);
	AddStatic(yAxis);
	AddStatic(zAxis);

	std::string skyBoxFiles[1] =
	{
		"/CUBE/skybox.png",
	};

	TextureCube* environment = new TextureCube("skybox", skyBoxFiles, 1);
	Shader* skybox = Shader::CreateFromFile("skybox", "/HLSL/03_skybox.hlsl");
	Material* skyboxMaterial = new Material(skybox);
	skybox->Bind();
	m_SkyboxMaterial = new MaterialInstance(skyboxMaterial);
	m_SkyboxMaterial->SetTexture("u_EnvironmentMap", environment);
	Entity* skyboxEntity = new Entity(MeshFactory::CreateQuad(-1, -1, 2, 2));
	skyboxEntity->SetMaterial(m_SkyboxMaterial);
	SetSkybox(skyboxEntity);




	
	std::string mapData = system::FileSystem::ReadTextFile("/JSON/MAP/map1.json");

	Json::Value root;
	Json::Reader reader;

	bool parsingSuccessful = reader.parse(mapData.c_str(), root);

	/*if (parsingSuccessful)
	{
		for (int i = 0; i < root.size(); ++i)
		{
			std::string name = root[i]["name"].asString();

			name[0] = toupper(name[0]);

			maths::vec3 translation = vec3(root[i]["position"]["x"].asFloat(), root[i]["position"]["y"].asFloat(), root[i]["position"]["z"].asFloat());
			maths::vec3 rotation    = vec3(root[i]["rotation"]["x"].asFloat(), root[i]["rotation"]["y"].asFloat(), root[i]["rotation"]["z"].asFloat());
			maths::vec3 scale       = vec3(root[i]["scale"]["x"].asFloat(), root[i]["scale"]["y"].asFloat(), root[i]["scale"]["z"].asFloat());

			mat4 position = mat4::Identity();

			auto a = new Model3D(name);

			m_mapObjects.push_back(a);

			//std::cout << a->GetIDColor() << std::endl;

			a->GetTransformComponent()->SetPosition(translation * 100);
			a->GetTransformComponent()->Rotate(rotation);
			a->GetTransformComponent()->SetScale(scale);
		
			
			AddStatic(a);
		}
	}*/


	Model* attack_basic = new Model("/SUN/Characters/20_attack_basic.sun");
	Model* idle_attack  = new Model("/SUN/Characters/20_idle_attack.sun");
	Model* idle_basic   = new Model("/SUN/Characters/20_idle_basic.sun");
	Model* roll_basic   = new Model("/SUN/Characters/20_roll_basic.sun");
	Model* run_attack   = new Model("/SUN/Characters/20_run_attack.sun");
	Model* run_basic    = new Model("/SUN/Characters/20_run_basic.sun");

	vector<Mesh*> animations;
	animations.push_back(attack_basic->GetMesh());
	animations.push_back(idle_attack->GetMesh());
	animations.push_back(idle_basic->GetMesh());
	animations.push_back(roll_basic->GetMesh());
	animations.push_back(run_attack->GetMesh());
	animations.push_back(run_basic->GetMesh());

	m_character = new Animation3D(animations, new Texture2D("/TEXTURE/20_body.png"), new Texture2D("/TEXTURE/20_face.png"));
	m_character->GetTransformComponent()->SetPosition(vec3(0, 0, 0));
	m_character->GetTransformComponent()->SetRotation(vec3(0, 0, 0));
	m_character->GetTransformComponent()->SetScale(vec3(100, 100, 100));
	SetCamera(new QuaterCamera(maths::mat4::Perspective(65.0f, 1600.0f / 900.0f, 0.1f, 1000.1f), m_character));

	Add(m_character);



}

void TestLayer3D::OnTick()
{

}

void TestLayer3D::OnUpdate(const utils::Timestep& ts)
{
	/*if (Input::IsKeyPressed(SUNNY_KEY_A))		m_character->GetTransformComponent()->Translate(vec3(-5, 0, 0));
	if (Input::IsKeyPressed(SUNNY_KEY_D))		m_character->GetTransformComponent()->Translate(vec3(5, 0, 0));
	if (Input::IsKeyPressed(SUNNY_KEY_W))		m_character->GetTransformComponent()->Translate(vec3(0, 0, 5));
	if (Input::IsKeyPressed(SUNNY_KEY_S))		m_character->GetTransformComponent()->Translate(vec3(0, 0, -5));

	if(!Input::IsKeyPressed(SUNNY_KEY_A) && !Input::IsKeyPressed(SUNNY_KEY_D) && !Input::IsKeyPressed(SUNNY_KEY_W) && !Input::IsKeyPressed(SUNNY_KEY_S))
		if (Input::IsMouseButtonPressed(SUNNY_MOUSE_LEFT))
			m_character->SetAnimation(0);
		else
			m_character->SetAnimation(2);
	else
	{
		if (Input::IsMouseButtonPressed(SUNNY_MOUSE_LEFT))
			m_character->SetAnimation(4);
		else
			m_character->SetAnimation(5);
	
	}

	m_character->PlayAnimation();
	m_character->Update();

	for (auto& b : m_bullets)
		b->Update();

	if (Input::IsMouseButtonPressed(SUNNY_MOUSE_LEFT) && m_character->Attack())
	{
	
		float x = m_character->GetTransformComponent()->GetPosition().x;
		float z = m_character->GetTransformComponent()->GetPosition().z;

		Bullet* b = new Bullet(m_sphere->GetMesh(), m_radian);
		b->GetTransformComponent()->SetPosition({x, 100, z});
		m_bullets.push_back(b);
		Add(b);
	}*/


	mat4 vp = GetCamera()->GetProjectionMatrix() * GetCamera()->GetViewMatrix();
	m_SkyboxMaterial->SetUniform("invViewProjMatrix", mat4::Invert(vp));


}

void TestLayer3D::OnEvent(Event& event)
{
	Layer3D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&TestLayer3D::OnKeyPressedEvent));
	dispatcher.Dispatch<KeyReleasedEvent>(METHOD(&TestLayer3D::OnKeyReleasedEvent));
	dispatcher.Dispatch<MousePressedEvent>(METHOD(&TestLayer3D::OnMousePressedEvent));
	dispatcher.Dispatch<MouseMovedEvent>(METHOD(&TestLayer3D::OnMouseMovedEvent));
	dispatcher.Dispatch<MouseReleasedEvent>(METHOD(&TestLayer3D::OnMouseReleasedEvent));
}

bool TestLayer3D::OnKeyPressedEvent(KeyPressedEvent& event)
{
	/*switch (event.GetKeyCode())
	{
	
		
	case SUNNY_KEY_SPACE:
		m_character->SetAnimation(3);
		m_character->SetRoll(true);

	}
	*/
	return false;
}

bool TestLayer3D::OnKeyReleasedEvent(KeyReleasedEvent& event)
{
	//if (event.GetKeyCode() == SUNNY_KEY_CONTROL) Renderer3D::DEFERRED_MODE = false;

	return false;
}

bool TestLayer3D::OnMousePressedEvent(MousePressedEvent& event)
{
	//std::cout << "pressed (" << event.GetX()  << ", " << event.GetY() << ")" << std::endl;

	float scaleX = Window::GetWindowClass()->GetResolutionWidth() / Window::GetWindowClass()->GetWidth();
	float scaleY = Window::GetWindowClass()->GetResolutionHeight() / Window::GetWindowClass()->GetHeight();

	maths::vec2 mouse(event.GetX() * scaleX, event.GetY() * scaleY);



	return false;
}

bool TestLayer3D::OnMouseReleasedEvent(MouseReleasedEvent& event)
{
	

	return false;
}

bool TestLayer3D::OnMouseMovedEvent(MouseMovedEvent& event)
{
	/*maths::vec2 mouse_xy(event.GetX(), event.GetY());

	vec3 P  = m_character->GetTransformComponent()->GetPosition();
	mat4 VP = GetCamera()->GetViewMatrix() * GetCamera()->GetProjectionMatrix();
	
	float x = P.x * VP.a11 + P.y * VP.a21 + P.z * VP.a31 + VP.a41;
	float y = P.x * VP.a12 + P.y * VP.a22 + P.z * VP.a31 + VP.a42;
	float z = P.x * VP.a13 + P.y * VP.a23 + P.z * VP.a31 + VP.a43;
	float w = P.x * VP.a14 + P.y * VP.a24 + P.z * VP.a31 + VP.a44;

	x /= w;
	y /= w;
	z /= w;

	double screenX = Window::GetWindowClass()->GetWidth()   * (x + 1.0f) / 2.0f + 0;
	double screenY = Window::GetWindowClass()->GetHeight()  * (2.0f - (y + 1.0f)) / 2.0f + 0;



	m_radian = maths::atan2(mouse_xy.y - screenY, mouse_xy.x -  screenX);

	m_degree = m_radian * 180 / maths::SUNNY_PI;

	m_character->GetTransformComponent()->SetRotation({ 0, m_degree + 90.0f , 0 });
	*/

	return false;
}