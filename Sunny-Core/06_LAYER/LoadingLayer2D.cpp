#include "LoadingLayer2D.h"


template <typename T> string tostr(const T& t) {
	ostringstream os;
	os << t;
	return os.str();
}

void StartLoad(LoadingLayer2D* layer);

LoadingLayer2D::LoadingLayer2D()
: Layer2D(maths::mat4::Orthographic(0.0f, Application::GetApplication().GetWindowWidth(), 0.0f, Application::GetApplication().GetWindowHeight(), -1.0f, 1.0f))
, m_loadingCompleted(false)
{

}

LoadingLayer2D::~LoadingLayer2D()
{
	m_thread->join();


	std::cout << "DELETE: LoadingLayer2D" << std::endl;

	delete m_backgroundTexture;
	delete m_background;
	delete m_circleTexture;
	for (auto circle : m_circles) delete circle;
	delete m_logoTexture;
	delete m_logo;
	delete m_state;
	delete m_ing;
}

void LoadingLayer2D::OnInit(Renderer2D& renderer)
{
	AudioEngine::LoadSound("04_ASSET/MP3/sunny_loading.mp3", false, true);
	AudioEngine::Play("04_ASSET/MP3/sunny_loading.mp3");
	m_backgroundTexture = new Texture2D("/TEXTURE/Loadings/background.png");
	m_background = new Sprite(0, 0, m_windowWidth, m_windowHeight, m_backgroundTexture);
	Add(m_background);

	m_circleTexture = new Texture2D("/TEXTURE/Loadings/circle.png");

	for (int i = 0; i < 13; ++i)
	{
		RoadingCircle* circle = new RoadingCircle(i * 160, 860 - 142 * 0, 10, m_circleTexture);
		m_circles.push_back(circle);
		Add(circle);
	}

	for (int i = 0; i < 11; ++i)
	{
		RoadingCircle* circle = new RoadingCircle(i * 160 + 80, 860 - 142 * 1, 10, m_circleTexture);
		m_circles.push_back(circle);
		Add(circle);
	}

	for (int i = 0; i < 13; ++i)
	{
		RoadingCircle* circle = new RoadingCircle(i * 160, 860 - 142 * 2, 10, m_circleTexture);
		m_circles.push_back(circle);
		Add(circle);
	}

	for (int i = 0; i < 11; ++i)
	{
		RoadingCircle* circle = new RoadingCircle(i * 160 + 80, 860 - 142 * 3, 10, m_circleTexture);
		m_circles.push_back(circle);
		Add(circle);
	}

	for (int i = 0; i < 13; ++i)
	{
		RoadingCircle* circle = new RoadingCircle(i * 160, 860 - 142 * 4, 10, m_circleTexture);
		m_circles.push_back(circle);
		Add(circle);
	}

	for (int i = 0; i < 11; ++i)
	{
		RoadingCircle* circle = new RoadingCircle(i * 160 + 80, 860 - 142 * 5, 10, m_circleTexture);
		m_circles.push_back(circle);
		Add(circle);
	}

	for (int i = 0; i < 13; ++i)
	{
		RoadingCircle* circle = new RoadingCircle(i * 160, 860 - 142 * 6, 10, m_circleTexture);
		m_circles.push_back(circle);
		Add(circle);
	}

	m_circles[0]->start = 10;
	m_circles[2]->start = 10;
	m_circles[5]->start = 10;
	m_circles[8]->start = 10;
	m_circles[14]->start = 10;
	m_circles[19]->start = 10;
	m_circles[22]->start = 10;
	m_circles[23]->start = 10;
	m_circles[24]->start = 10;
	m_circles[35]->start = 10;
	m_circles[37]->start = 10;
	m_circles[38]->start = 10;
	m_circles[42]->start = 10;
	m_circles[44]->start = 10;
	m_circles[46]->start = 10;
	m_circles[48]->start = 10;
	m_circles[51]->start = 10;
	m_circles[52]->start = 10;
	m_circles[53]->start = 10;
	m_circles[55]->start = 10;
	m_circles[57]->start = 10;
	m_circles[59]->start = 10;
	m_circles[60]->start = 10;
	m_circles[66]->start = 10;
	m_circles[69]->start = 10;
	m_circles[71]->start = 10;
	m_circles[73]->start = 10;

	m_circles[4]->start = 17;
	m_circles[6]->start = 17;
	m_circles[10]->start = 17;
	m_circles[15]->start = 17;
	m_circles[21]->start = 17;
	m_circles[25]->start = 17;
	m_circles[27]->start = 17;
	m_circles[50]->start = 17;
	m_circles[40]->start = 17;
	m_circles[52]->start = 17;

	m_logoTexture = new Texture2D("/TEXTURE/Loadings/boo2.png");
	m_logo = new Sprite(m_windowWidth/ 2 , m_windowHeight / 2 + 65, m_logoTexture, PIVOT_CENTER);
	
	m_panel = new Panel();

	m_state = new Label("LOADING", m_windowWidth / 2 - 90, 107, "power", RGBA(1, 1, 1, 0.9), Label::Alignment::LEFT);
	Add(m_state);
	m_ing = new Label("Initialize", m_windowWidth / 2, 73, "power", 16, RGBA(0.9, 0.4, 0.4, 1.0), Label::Alignment::CENTER);
	m_loadingBar = new Loadingbar(maths::Rectangle(400, 65, 400, 15), m_ing, RGBA(0.5, 0.5, 0.5, 0.7), RGBA(0.9, 0.9, 0.9, 0.7));
	m_loadingBar->SetCallback(METHOD(&LoadingLayer2D::OnLoadingCompleted));
	m_loadingBar->SetValue(0);

	m_panel->Add(m_loadingBar);
}

void LoadingLayer2D::OnTick()
{
	m_tickCount++;

	if(m_tickCount == 1)
		m_thread = new  thread(StartLoad, this);

	if (!m_loadingCompleted)
	{
		string a = "Loading";
		for (int i = 0; i < m_tickCount; ++i)
			a += ".";
		m_state->SetText(a);
	}

}

void LoadingLayer2D::OnUpdate(const utils::Timestep& ts)
{
	m_updateCount++;

	for (auto circle : m_circles)
	{
		circle->Update();
	}

	if (m_loadingCompleted &&  m_updateCount >= 50)
	{
		if (m_state->GetVisible())
			m_state->SetVisible(false);
		else
			m_state->SetVisible(true);

		m_updateCount = 0;
	}
}

void LoadingLayer2D::OnRender(Renderer2D& renderer)
{

}

void LoadingLayer2D::OnEvent(Event& event)
{
	Layer2D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MousePressedEvent>(METHOD(&LoadingLayer2D::OnMousePressedEvent));
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&LoadingLayer2D::OnKeyPressedEvent));
}

bool LoadingLayer2D::OnMousePressedEvent(MousePressedEvent& event)
{
	GoToRoom();
	return false;
}

bool LoadingLayer2D::OnKeyPressedEvent(KeyPressedEvent& event)
{
	GoToRoom();
	return false;
}

void LoadingLayer2D::OnLoadingCompleted(float f)
{
	if (!m_loadingCompleted)
	{
		m_loadingCompleted = true;

		m_state->SetText("Press any Key to start game");
		m_state->SetAlignment(Label::Alignment::CENTER);
		m_state->SetPosition(vec2(m_windowWidth / 2, 107));
		m_state->SetColor(RGBA(1.f, 1.f, 1.f, 1.f));

		m_ing->SetText("Completed");

		delete m_panel;
	}
}

void LoadingLayer2D::GoToRoom()
{
	if (m_loadingCompleted == false) return;

	delete Application::GetApplication().PopLayer(this);
	Application::GetApplication().PushLayer2D(new RoomLayer2D());
}

void StartLoad(LoadingLayer2D* layer)
{
	float count = 0;
	float ex = 0;

	Json::Value  root;
	Json::Reader reader;
	std::string  name, path, texture_path;
	int textureLen;

	Model* a;

	unordered_map<std::string, std::string> jsons;
	jsons["Characters"] = sunny::system::FileSystem::ReadTextFile("/JSON/CHARACTER/Characters.json");

	jsons["Trees"] = sunny::system::FileSystem::ReadTextFile("/JSON/MODEL/Trees.json");
	jsons["Map"] = sunny::system::FileSystem::ReadTextFile("/JSON/MODEL/LowPolyNatures.json");

	for (auto& obj : jsons)
	{
		if (reader.parse(obj.second.c_str(), root)) ex += root.size();
	}

	for (auto& obj : jsons)
	{
		reader.parse(obj.second.c_str(), root);

		for (int i = 0; i < root.size(); ++i)
		{
			ModelData* data = new ModelData();

			name = root[i]["name"].asString();
			path = root[i]["path"].asString();
			textureLen = root[i]["textures"].size();


			for (int texture_i = 0; texture_i < textureLen; ++texture_i)
			{
				texture_path = root[i]["textures"][texture_i].asString();

				data->textures.push_back(texture_path);

				Application::GetApplication().m_mutex.lock();

				if (!sunny::graphics::TextureManager::Get(texture_path))
					sunny::graphics::TextureManager::Add(new sunny::directx::Texture2D(texture_path));

				Application::GetApplication().m_mutex.unlock();
			}

			


			if (!sunny::graphics::ModelManager::Get(name))
			{
				Application::GetApplication().m_mutex.lock();

				a = new sunny::graphics::Model(path);

				sunny::graphics::ModelManager::Add(name, a);
				
				Application::GetApplication().m_mutex.unlock();
			}


			AssetData::AddModelData(name, data);

			layer->m_loadingBar->SetValue(++count / ex);

			layer->m_ing->SetText(obj.first + " Initialize (" + tostr(count) + "/" + tostr(ex) + string(")"));
		}
	}
}