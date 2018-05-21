#include "LoadingLayer2D.h"


template <typename T> string tostr(const T& t) {
	ostringstream os;
	os << t;
	return os.str();
}

void StartLoad(LoadingLayer2D* layer);

LoadingLayer2D::LoadingLayer2D()
: Layer2D(maths::mat4::Orthographic(0.0f, Application::GetApplication().GetWindowWidth(), 0.0f, Application::GetApplication().GetWindowHeight(), -1.0f, 1.0f))
{

}

LoadingLayer2D::~LoadingLayer2D()
{
	m_thread->join();

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
	//Add(m_logo);
	
	m_panel = new Panel();

	m_state = new Label("LOADING...", m_windowWidth / 2, 107, "power", RGBA(1, 1, 1, 0.9), Label::Alignment::CENTER);
	Add(m_state);
	m_ing = new Label("Initialize", m_windowWidth / 2, 73, "power", 16, RGBA(0.9, 0.5, 0.5, 0.9), Label::Alignment::CENTER);
	m_loadingBar = new Loadingbar(maths::Rectangle(400, 65, 400, 15), m_ing, RGBA(0.5, 0.5, 0.5, 0.7), RGBA(0.9, 0.9, 0.9, 0.7));
	m_loadingBar->SetValue(0);



	m_panel->Add(m_loadingBar);


	//std::async(std::launch::deferred, al, this);

}

void LoadingLayer2D::OnTick()
{
	m_tickCount++;

	if(m_tickCount == 1)
		m_thread = new  thread(StartLoad, this);

}

void LoadingLayer2D::OnUpdate(const utils::Timestep& ts)
{
	for (auto circle : m_circles)
	{
		circle->Update();
	}
	//m_circle->SetSize(vec2(m_circle->GetSize().x + 1, m_circle->GetSize().y + 1));
}

void LoadingLayer2D::OnRender(Renderer2D& renderer)
{

}

void LoadingLayer2D::OnEvent(Event& event)
{

}

bool LoadingLayer2D::OnMouseMovedEvent(MouseMovedEvent& event)
{

}

bool LoadingLayer2D::OnMousePressedEvent(MousePressedEvent& event)
{

}

bool LoadingLayer2D::OnKeyPressedEvent(KeyPressedEvent& event)
{

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

	jsons["Trees"] = sunny::system::FileSystem::ReadTextFile("/JSON/MODEL/Trees.json");
	jsons["Map"] = sunny::system::FileSystem::ReadTextFile("/JSON/MODEL/LowPolyNatures.json");
	jsons["Characters"] = sunny::system::FileSystem::ReadTextFile("/JSON/CHARACTER/Characters.json");

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

			Application::GetApplication().m_mutex.lock();

			for (int texture_i = 0; texture_i < textureLen; ++texture_i)
			{
				texture_path = root[i]["textures"][texture_i].asString();

				data->textures.push_back(texture_path);

				if (!sunny::graphics::TextureManager::Get(texture_path))
					sunny::graphics::TextureManager::Add(new sunny::directx::Texture2D(texture_path));
			}


			if (!sunny::graphics::ModelManager::Get(name))
			{
				a = new sunny::graphics::Model(path, false);

				sunny::graphics::ModelManager::Add(name, a);

				a->LoadMesh();

			}

			Application::GetApplication().m_mutex.unlock();

			AssetData::AddModelData(name, data);

			layer->m_loadingBar->SetValue(++count / ex);

			layer->m_ing->SetText(obj.first + " Initialize (" + tostr(count) + "/" + tostr(ex) + string(")"));


		}
	}
}