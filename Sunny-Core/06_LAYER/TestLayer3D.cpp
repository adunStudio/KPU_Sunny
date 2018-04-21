#include "TestLayer3D.h"

TestLayer3D::TestLayer3D()
{

}

TestLayer3D::~TestLayer3D()
{

}

void TestLayer3D::OnInit(Renderer3D& renderer)
{
	m_mousePicker = new MousePicker(GetCamera());

	LightSetup* lights = new LightSetup();
	Light* light = new Light(vec3(0.3, 0.3, 0.3), 2, vec4(1.f, 1.f, 1.f, 1.f));

	lights->Add(light);

	PushLightSetup(lights);

	std::string skyBoxFiles[1] =
	{
		"/CUBE/CubeMap1.tga",
		/*"/CUBE/CubeMap2.tga",
		"/CUBE/CubeMap3.tga",
		"/CUBE/CubeMap4.tga",
		"/CUBE/CubeMap5.tga",
		"/CUBE/CubeMap6.tga",
		"/CUBE/CubeMap7.tga",
		"/CUBE/CubeMap8.tga",
		"/CUBE/CubeMap9.tga",
		"/CUBE/CubeMap10.tga",*/
	};

	TextureCube* environment = new TextureCube("skybox", skyBoxFiles, 1);
	Shader* skybox = Shader::CreateFromFile("skybox", "/HLSL/skybox.hlsl");
	Material* skyboxMaterial = new Material(skybox);
	skybox->Bind();
	m_SkyboxMaterial = new MaterialInstance(skyboxMaterial);
	m_SkyboxMaterial->SetTexture("u_EnvironmentMap", environment);
	Entity* skyboxEntity = new Entity(MeshFactory::CreateQuad(-1, -1, 2, 2));
	skyboxEntity->SetMaterial(m_SkyboxMaterial);
	Add(skyboxEntity);

	
	Entity* xAxis = new Entity(MeshFactory::CreateXAxis(), RGBA(1, 0, 0, 1), mat4::Identity() * mat4::Scale(vec3(100, 100, 100)));
	Entity* yAxis = new Entity(MeshFactory::CreateYAxis(), RGBA(0, 1, 0, 1), mat4::Identity() * mat4::Scale(vec3(100, 100, 100)));
	Entity* zAxis = new Entity(MeshFactory::CreateZAxis(), RGBA(0, 0, 1, 1), mat4::Identity() * mat4::Scale(vec3(100, 100, 100)));
	Add(xAxis);
	Add(yAxis);
	Add(zAxis);


	Tree* tree1  = new Tree("DeadOak1",    mat4::Identity() * mat4::Translate(vec3(-50, 0, 0)));
	Tree* tree2  = new Tree("DeadOak2",    mat4::Identity() * mat4::Translate(vec3(-40, 0, 0)));
	Tree* tree3  = new Tree("DeadOak2",    mat4::Identity() * mat4::Translate(vec3(-30, 0, 0)));
	Tree* tree4  = new Tree("DeadSpruce1", mat4::Identity() * mat4::Translate(vec3(-20, 0, 0)));
	Tree* tree5  = new Tree("DeadSpruce2", mat4::Identity() * mat4::Translate(vec3(-10, 0, 0)));
	Tree* tree6  = new Tree("DeadSpruce3", mat4::Identity() * mat4::Translate(vec3(  0, 0, 0)));
	Tree* tree7  = new Tree("OakTree1",    mat4::Identity() * mat4::Translate(vec3( 10, 0, 0)));
	Tree* tree8  = new Tree("OakTree2",    mat4::Identity() * mat4::Translate(vec3( 20, 0, 0)));
	Tree* tree9  = new Tree("OakTree3",    mat4::Identity() * mat4::Translate(vec3( 30, 0, 0)));
	Tree* tree10 = new Tree("SpruceTree1", mat4::Identity() * mat4::Translate(vec3( 40, 0, 0)));
	Tree* tree11 = new Tree("SpruceTree2", mat4::Identity() * mat4::Translate(vec3( 50, 0, 0)));
	Tree* tree12 = new Tree("SpruceTree3", mat4::Identity() * mat4::Translate(vec3( 60, 0, 0)));

	
	std::string mapData = system::FileSystem::ReadTextFile("/JSON/MAP/map1.json");

	Json::Value root;
	Json::Reader reader;

	bool parsingSuccessful = reader.parse(mapData.c_str(), root);

	if (parsingSuccessful)
	{
		for (int i = 0; i < root.size(); ++i)
		{
			std::string name = root[i]["name"].asString();

			name[0] = toupper(name[0]);

			maths::vec3 translation = vec3(root[i]["position"]["x"].asFloat(), root[i]["position"]["y"].asFloat(), root[i]["position"]["z"].asFloat());
			maths::vec3 rotation = vec3(root[i]["rotation"]["x"].asFloat(), root[i]["rotation"]["y"].asFloat(), root[i]["rotation"]["z"].asFloat());
			maths::vec3 scale = vec3(root[i]["scale"]["x"].asFloat(), root[i]["scale"]["y"].asFloat(), root[i]["scale"]["z"].asFloat());

			mat4 position = mat4::Identity();



			auto a = new Model3D(name);

			std::cout << a->GetIDColor() << std::endl;

			a->GetTransformComponent()->Rotate(rotation.z, vec3(0, 0, 1));
			a->GetTransformComponent()->Rotate(rotation.y, vec3(0, 1, 0));
			a->GetTransformComponent()->Rotate(rotation.x, vec3(1, 0, 0));
			a->GetTransformComponent()->SetPosition(translation * 100);

			a->GetTransformComponent()->SetScale(scale);
			
			Add(a);
		}
	}

	Model* a = new Model("/OBJ/sphere.obj");

	Entity* e = new Entity(a->GetMesh(), RGBA(1.0, 0, 0, 1.0));
	Add(e);
	/*
	Add(tree1);
	Add(tree2);
	Add(tree3);
	Add(tree4);
	Add(tree5);
	Add(tree6);
	Add(tree7);
	Add(tree8);
	Add(tree9);
	Add(tree10);
	Add(tree11);
	Add(tree12);*/

}

void TestLayer3D::OnTick()
{

}

void TestLayer3D::OnUpdate(const utils::Timestep& ts)
{
	mat4 vp = GetCamera()->GetProjectionMatrix() * GetCamera()->GetViewMatrix();
	m_SkyboxMaterial->SetUniform("invViewProjMatrix", mat4::Invert(vp));
}

void TestLayer3D::OnEvent(Event& event)
{
	Layer3D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&TestLayer3D::OnKeyPressedEvent));
	dispatcher.Dispatch<MousePressedEvent>(METHOD(&TestLayer3D::OnMousePressedEvent));
}

bool TestLayer3D::OnKeyPressedEvent(KeyPressedEvent& event)
{

	if (event.GetKeyCode() == SUNNY_KEY_1 && !event.GetRepeat()) Renderer3D::DEFERRED_MODE = !Renderer3D::DEFERRED_MODE;

	return false;
}

bool TestLayer3D::OnMousePressedEvent(MousePressedEvent& event)
{
	//std::cout << "pressed (" << event.GetX()  << ", " << event.GetY() << ")" << std::endl;

	float scaleX = Window::GetWindowClass()->GetResolutionWidth() / Window::GetWindowClass()->GetWidth();
	float scaleY = Window::GetWindowClass()->GetResolutionHeight() / Window::GetWindowClass()->GetHeight();

	maths::vec2 mouse(event.GetX() * scaleX, event.GetY() * scaleY);

	m_mousePicker->Update(mouse);

	std::cout << m_mousePicker->GetRay() << std::endl;

	//float x = event.GetX() - scaleX;
	//float y = event.GetY() - scaleY;
	// std::cout << mouse << std::endl;

	//mouse.x /= (Window::GetWindowClass()->GetWidth() / 2);
	//mouse.y /= (Window::GetWindowClass()->GetHeight() / 2);

	//std::cout << mouse << std::endl;

	/*vec3 view = GetCamera()->GetFocalPoint() - GetCamera()->GetPosition();
	view = view.Normalize();

	vec3 h = view.Cross(vec3(0, 1, 0));
	h = h.Normalize();

	vec3 v = h.Cross(view);
	v = v.Normalize();*/

	//std::cout << v << std::endl;

	return false;
}