#include "MapGUILayer3D.h"

template <typename T> string tostr(const T& t) {
	ostringstream os;
	os << t;
	return os.str();
}

MapGUILayer3D::MapGUILayer3D()
{
	m_layer2D = new MapGUILayer2D(this);

	Application::GetApplication().PushLayer2D(m_layer2D);

	
}

MapGUILayer3D::~MapGUILayer3D()
{

}

void MapGUILayer3D::OnInit(Renderer3D& renderer)
{
	Model* a = new Model("/OBJ/sphere.obj");
	Entity* e = new Entity(a->GetMesh(), RGBA(1.0, 0, 0, 1.0));
	Add(e);


	LightSetup* lights = new LightSetup();
	Light* light = new Light(vec3(0.3, 0.3, 0.3), 2, vec4(1.f, 1.f, 1.f, 1.f));

	lights->Add(light);

	PushLightSetup(lights);


	Entity* xAxis = new Entity(MeshFactory::CreateXAxis(), RGBA(1, 0, 0, 0), mat4::Identity() * mat4::Scale(vec3(100, 100, 100)));
	Entity* yAxis = new Entity(MeshFactory::CreateYAxis(), RGBA(0, 1, 0, 0), mat4::Identity() * mat4::Scale(vec3(100, 100, 100)));
	Entity* zAxis = new Entity(MeshFactory::CreateZAxis(), RGBA(0, 0, 1, 0), mat4::Identity() * mat4::Scale(vec3(100, 100, 100)));
	Add(xAxis);
	Add(yAxis);
	Add(zAxis);

	model_axis = new Group3D();

	model_x = new Entity(MeshFactory::CreateXAxis(), RGBA(1, 0, 0, 0));
	model_y = new Entity(MeshFactory::CreateYAxis(), RGBA(0, 1, 0, 0));
	model_z = new Entity(MeshFactory::CreateZAxis(), RGBA(0, 0, 1, 0));

	model_axis->Add(model_x);
	model_axis->Add(model_y);
	model_axis->Add(model_z);






	model_axis->GetTransformComponent()->SetScale(vec3(300, 300, 300));
	
	Add(model_axis);



	std::string mapData = system::FileSystem::ReadTextFile("/JSON/MAP/map5.json");

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

			m_mapObjects.push_back(a);

			a->GetTransformComponent()->SetPosition(translation);
			a->GetTransformComponent()->Rotate(rotation);
			a->GetTransformComponent()->SetScale(scale);

			Add(a);
		}
	}

	Mesh* g = MeshFactory::CreateGeometry(7000, 7000, 100, 100);

	Entity* geom = new Entity(g, new Texture2D("/TEXTURE/terrain_basic.png"));

	Add(geom);
}

void MapGUILayer3D::OnTick()
{

}

void MapGUILayer3D::OnUpdate(const utils::Timestep& ts)
{
	m_layer2D->camera_position = GetCamera()->GetPosition();
	m_layer2D->camera_focalPoint = GetCamera()->GetFocalPoint();

	if (m_pickedModel)
	{
		auto a = m_pickedModel->GetTransformComponent();

		auto t = a->GetPosition();
		auto r = a->GetRotation();
		auto s = a->GetScale();

	
		model_axis->GetTransformComponent()->SetPosition(a->GetPosition() + m_pickedModel->GetMesh()->center);
	}

}

void MapGUILayer3D::OnEvent(Event& event)
{
	Layer3D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&MapGUILayer3D::OnKeyPressedEvent));
	dispatcher.Dispatch<KeyReleasedEvent>(METHOD(&MapGUILayer3D::OnKeyReleasedEvent));
	dispatcher.Dispatch<MousePressedEvent>(METHOD(&MapGUILayer3D::OnMousePressedEvent));
	dispatcher.Dispatch<MouseReleasedEvent>(METHOD(&MapGUILayer3D::OnMouseReleasedEvent));
	dispatcher.Dispatch<MouseMovedEvent>(METHOD(&MapGUILayer3D::OnMouseMovedEvent));
}


bool MapGUILayer3D::OnKeyPressedEvent(KeyPressedEvent& event)
{

	if (!m_pickedModel) return false;

	auto a = m_pickedModel->GetTransformComponent();

	/*if (event.GetKeyCode() == SUNNY_KEY_W) a->Translate(vec3(0, 0, 10));
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
	}*/

	return false;
}

bool MapGUILayer3D::OnKeyReleasedEvent(KeyReleasedEvent& event)
{
	//if (event.GetKeyCode() == SUNNY_KEY_CONTROL) Renderer3D::DEFERRED_MODE = false;

	return false;
}

bool MapGUILayer3D::OnMousePressedEvent(MousePressedEvent& event)
{
	float scaleX = Window::GetWindowClass()->GetResolutionWidth() / Window::GetWindowClass()->GetWidth();
	float scaleY = Window::GetWindowClass()->GetResolutionHeight() / Window::GetWindowClass()->GetHeight();

	maths::vec2 mouse(event.GetX() * scaleX, event.GetY() * scaleY);

	prevMouse = mouse;

	const unsigned char* diffuseData = directx::GeometryBuffer::GetIDData();
	int rowPitch = directx::GeometryBuffer::GetMapRowPitch();

	int col = event.GetX();
	int row = /*Window::GetWindowClass()->GetHeight() - */event.GetY();

	int rowStart = row * rowPitch;
	int colStart = col * 4;


	unsigned char R = diffuseData[rowStart + colStart + 0];
	unsigned char G = diffuseData[rowStart + colStart + 1];
	unsigned char B = diffuseData[rowStart + colStart + 2];
	unsigned char A = diffuseData[rowStart + colStart + 3];

	unsigned int aa = (A << 24) | (R << 16) | (G << 8) | B;
	unsigned int id = R + G * 256 + B * 256 * 256;

	if (!Input::IsKeyPressed(SUNNY_KEY_CONTROL))
	{
		if (m_pickedModel)
		{
			if (id == model_x->GetID())
				x_position_mode = true;

			if (id == model_y->GetID())
				y_position_mode = true;

			if (id == model_z->GetID())
				z_position_mode = true;
		}

		return false;
	}


	for (auto object : m_mapObjects)
	{
		if (object->GetID() == id)
		{
			if (m_pickedModel)
				m_pickedModel->picked = false;

			object->picked = true;
			m_pickedModel = object;
			m_layer2D->SetPickedModel(m_pickedModel);
		}
	}
	
	return false;
}

bool MapGUILayer3D::OnMouseMovedEvent(MouseMovedEvent& event)
{
	float scaleX = Window::GetWindowClass()->GetResolutionWidth() / Window::GetWindowClass()->GetWidth();
	float scaleY = Window::GetWindowClass()->GetResolutionHeight() / Window::GetWindowClass()->GetHeight();

	maths::vec2 mouse(event.GetX() * scaleX, event.GetY() * scaleY);


	if (x_position_mode)
		m_layer2D->model_position.x += (mouse.x - prevMouse.x) * 2;
	if (y_position_mode)
		m_layer2D->model_position.y += (prevMouse.y - mouse.y) * 2;
	if (z_position_mode)
		m_layer2D->model_position.z += (prevMouse.y - mouse.y) * 2;


	prevMouse = mouse;

	return false;
}


bool MapGUILayer3D::OnMouseReleasedEvent(MouseReleasedEvent& event)
{
	x_position_mode = false;
	y_position_mode = false;
	z_position_mode = false;

	return false;
}

