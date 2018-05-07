#include "TestLayer3D.h"
#include "TestLayer2D.h"

template <typename T> string tostr(const T& t) {
	ostringstream os;
	os << t;
	return os.str();
}

TestLayer3D::TestLayer3D(TestLayer2D& layer)
: m_testLayer2D(layer)
{

}

TestLayer3D::~TestLayer3D()
{

}

void TestLayer3D::OnInit(Renderer3D& renderer)
{
	Model* a = new Model("/OBJ/sphere.obj");
	Entity* e = new Entity(a->GetMesh(), RGBA(1.0, 0, 0, 1.0));
	Add(e);

	m_mousePicker = new MousePicker(GetCamera());

	LightSetup* lights = new LightSetup();
	Light* light = new Light(vec3(0.3, 0.3, 0.3), 2, vec4(1.f, 1.f, 1.f, 1.f));

	lights->Add(light);

	PushLightSetup(lights);


	Entity* xAxis = new Entity(MeshFactory::CreateXAxis(), RGBA(1, 0, 0, 1), mat4::Identity() * mat4::Scale(vec3(100, 100, 100)));
	Entity* yAxis = new Entity(MeshFactory::CreateYAxis(), RGBA(0, 1, 0, 1), mat4::Identity() * mat4::Scale(vec3(100, 100, 100)));
	Entity* zAxis = new Entity(MeshFactory::CreateZAxis(), RGBA(0, 0, 1, 1), mat4::Identity() * mat4::Scale(vec3(100, 100, 100)));
	Add(xAxis);
	Add(yAxis);
	Add(zAxis);

	std::string skyBoxFiles[1] =
	{
		"/CUBE/skybox.png",
	};

	TextureCube* environment = new TextureCube("skybox", skyBoxFiles, 1);
	Shader* skybox = Shader::CreateFromFile("skybox", "/HLSL/skybox.hlsl");
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

	if (parsingSuccessful)
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
		
			
			Add(a);
		}
	}


	Model* m = new Model("/SUN/kick.sun");
	e2 = new Entity(m->GetMesh(), new Texture2D("/TEXTURE/test.png"), new Texture2D("/TEXTURE/face.png"), mat4::Identity() * mat4::Translate(vec3(-300, 0, 100)) * mat4::Scale(vec3(100, 100, 100)));

	Add(e2);
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
	e2->PlayAnimation();

	mat4 vp = GetCamera()->GetProjectionMatrix() * GetCamera()->GetViewMatrix();
	m_SkyboxMaterial->SetUniform("invViewProjMatrix", mat4::Invert(vp));

	if (m_pickedModel)
	{
		auto a = m_pickedModel->GetTransformComponent();
		
		auto t = a->GetPosition();
		auto r = a->GetRotation();
		auto s = a->GetScale();

		m_testLayer2D.model_position->SetText("[T] x: " + tostr(t.x) + " \t\t y: " + tostr(t.y) + " \t\t z:" + tostr(t.z) + " \t");
		m_testLayer2D.model_rotation->SetText("[R] x: " + tostr(r.x) + " \t\t y: " + tostr(r.y) + " \t\t z:" + tostr(r.z) + " \t");
		m_testLayer2D.model_scale->SetText("[S] x: " +tostr(s.x)+ " \t\t y: " + tostr(s.y) + " \t\t z:" + tostr(s.z) + " \t");
	}

}

void TestLayer3D::OnEvent(Event& event)
{
	Layer3D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&TestLayer3D::OnKeyPressedEvent));
	dispatcher.Dispatch<KeyReleasedEvent>(METHOD(&TestLayer3D::OnKeyReleasedEvent));
	dispatcher.Dispatch<MousePressedEvent>(METHOD(&TestLayer3D::OnMousePressedEvent));
	dispatcher.Dispatch<MouseReleasedEvent>(METHOD(&TestLayer3D::OnMouseReleasedEvent));
}

bool TestLayer3D::OnKeyPressedEvent(KeyPressedEvent& event)
{

	if (!m_pickedModel) return false;

	auto a = m_pickedModel->GetTransformComponent();

	if (event.GetKeyCode() == SUNNY_KEY_W) a->Translate(vec3(0, 0, 10));
	if (event.GetKeyCode() == SUNNY_KEY_S) a->Translate(vec3(0, 0, -10));
	if (event.GetKeyCode() == SUNNY_KEY_A) a->Translate(vec3(-10, 0, 0));
	if (event.GetKeyCode() == SUNNY_KEY_D) a->Translate(vec3(10, 0, 0));

	if (event.GetKeyCode() == SUNNY_KEY_O) a->SetScale(a->GetScale() + 0.5);
	if (event.GetKeyCode() == SUNNY_KEY_P) a->SetScale(a->GetScale() - 0.5);

	if (Input::IsKeyPressed(SUNNY_KEY_SHIFT))
	{
		if (event.GetKeyCode() == SUNNY_KEY_1) a->Rotate(vec3(1, 0, 0));
		if (event.GetKeyCode() == SUNNY_KEY_2) a->Rotate(vec3(0, 1, 0));
		if (event.GetKeyCode() == SUNNY_KEY_3) a->Rotate(vec3(0, 0, 1));
	}
	else
	{
		if (event.GetKeyCode() == SUNNY_KEY_1) a->Rotate(vec3(-1, 0, 0));
		if (event.GetKeyCode() == SUNNY_KEY_2) a->Rotate(vec3(0, -1, 0));
		if (event.GetKeyCode() == SUNNY_KEY_3) a->Rotate(vec3(0, 0, -1));
	}
	
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
	//m_mousePicker->Update(mouse);

	//std::cout << m_mousePicker->GetRay() << std::endl;

	// 성능 저하의 원인 (레이캐스트 피킹방식으로 바꿔야 한다.)
	const unsigned char* diffuseData = directx::GeometryBuffer::GetIDData();
	int rowPitch = directx::GeometryBuffer::GetMapRowPitch();

	int col = event.GetX();
	int row = /*Window::GetWindowClass()->GetHeight() -*/ event.GetY();

	int rowStart = row * rowPitch;
	int colStart = col * 4;


	unsigned char R = diffuseData[rowStart + colStart + 0];
	unsigned char G = diffuseData[rowStart + colStart + 1];
	unsigned char B = diffuseData[rowStart + colStart + 2];
	unsigned char A = diffuseData[rowStart + colStart + 3];

	unsigned int aa = (A << 24) | (R << 16) | (G << 8) | B;
	unsigned int id = R + G * 256 + B * 256 * 256;

	//std::cout << id << std::endl;

	for (auto object : m_mapObjects)
	{
		if (object->GetID() == id)
			m_pickedModel = object;
	}

	return false;
}

bool TestLayer3D::OnMouseReleasedEvent(MouseReleasedEvent& event)
{
	for (auto object : m_mapObjects)
	{
		object->picked = false;
	}

	return false;
}