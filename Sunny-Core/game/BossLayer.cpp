#include "BossLayer.h"

BossLayer::BossLayer()
{

}

BossLayer::~BossLayer()
{

}

void BossLayer::OnInit(Renderer3D& renderer)
{

	std::string strJson =  system::FileSystem::ReadTextFile("resource/json/map.json");

	std::cout << strJson << std::endl;

	Json::Value root;
	Json::Reader reader;

	bool parsingSuccessful = reader.parse(strJson.c_str(), root);

	if (parsingSuccessful)
	{
		std::cout << "파싱 성공" << std::endl;

		for (int i = 0; i < root.size(); ++i)
		{
			std::string name = root[i]["name"].asString();
			maths::vec3 translation = vec3(root[i]["translation"]["x"].asFloat(), root[i]["translation"]["y"].asFloat(), root[i]["translation"]["z"].asFloat());
			maths::vec3 rotation    = vec3(root[i]["rotation"]["x"].asFloat()   , root[i]["rotation"]["y"].asFloat()   , root[i]["rotation"]["z"].asFloat()   );
			maths::vec3 scale       = vec3(root[i]["scale"]["x"].asFloat()      , root[i]["scale"]["y"].asFloat()      , root[i]["scale"]["z"].asFloat()      );
		
			if(!m_models[name])
				m_models[name] = new Model("/sun/" + name + ".sun");
		
			mat4 position = mat4::Identity() * mat4::Translate(translation);
			if (rotation.x != 0) position = position *  mat4::Rotate(rotation.x, vec3(1, 0, 0));
			if (rotation.y != 0) position = position *  mat4::Rotate(rotation.y, vec3(0, 1, 0));
			if (rotation.z != 0) position = position *  mat4::Rotate(rotation.z, vec3(0, 0, 1));
			position = position * mat4::Scale(scale);

			m_characters.push_back(new Entity(m_models[name]->GetMesh(), RGBA(0.5, 0.5, 0.5, 1), position));
		}

		for (auto& c : m_characters)
		{
			Add(c);
		}
		
	}
	else
	{
		std::cout << "Failed to parse"
			<< reader.getFormattedErrorMessages();

		exit(1);
	}


	/*
	m_models["thief_kick"] = new Model("/sun/thief_kick.sun");
	m_models["thief_dance"] = new Model("/sun/thief_dance.sun");
	m_models["thief_punch"] = new Model("/sun/thief_punch.sun");
	m_models["girl_run"] = new Model("/sun/girl_run.sun");

	m_characters.push_back(new Entity(m_models["thief_kick"]->GetMesh(), RGBA(0.5, 0.5, 0.5, 1), mat4::Identity() * mat4::Translate(vec3(-20, 0, 0)) * mat4::Rotate(90, vec3(0, 1, 0)) * mat4::Scale(vec3(0.3, 0.3, 0.3))));
	m_characters.push_back(new Entity(m_models["thief_dance"]->GetMesh(), RGBA(0.5, 0.5, 0.5, 1), mat4::Identity() * mat4::Translate(vec3(-70, 0, 0)) * mat4::Rotate(90, vec3(0, 1, 0)) * mat4::Scale(vec3(0.3, 0.3, 0.3))));
	m_characters.push_back(new Entity(m_models["thief_punch"]->GetMesh(), RGBA(0.5, 0.5, 0.5, 1), mat4::Identity() * mat4::Translate(vec3(-100, 0, 0)) * mat4::Rotate(90, vec3(0, 1, 0)) * mat4::Scale(vec3(0.3, 0.3, 0.3))));
	m_characters.push_back(new Entity(m_models["girl_run"]->GetMesh(), RGBA(0.5, 0.5, 0.5, 1), mat4::Identity() * mat4::Translate(vec3(60, 0, 20)) * mat4::Rotate(-90, vec3(1, 0, 0)) * mat4::Scale(vec3(0.02, 0.02, 0.02))));
	*/

	// 셰이더
	ShaderManager::Add(Shader::CreateFromFile("basic_light", std::string("resource/hlsl/basic_light.hlsl")));
	ShaderManager::Add(Shader::CreateFromFile("geometry", std::string("resource/hlsl/geometry.hlsl")));
	ShaderManager::Add(Shader::CreateFromFile("terrain",     std::string("resource/hlsl/terrain.hlsl")    ));

	// 머터리얼, 머터리얼 인스턴스
	Material* lightMaterial = new Material(ShaderManager::Get("geometry"));
	MaterialInstance* lightMaterialInstance = new MaterialInstance(lightMaterial);

	Material* terrainMaterial = new Material(ShaderManager::Get("terrain"));
	MaterialInstance* terrainMaterialInstance = new MaterialInstance(terrainMaterial);

	// 전역 조명 세팅
	LightSetup* lights = new LightSetup();
	Light* light = new Light(vec3(0.5, 0.5, 0.5), 1, vec4(1.f, 1.f, 1.f, 1.f));
	lights->Add(light);
	PushLightSetup(lights);

	// 맵 세팅
	m_heightMap = new HeightMap(500, 500, "/raw/terrain.raw");
	Model* terrain_model = new Model("/obj/terrain.obj");
	m_terrain = new Terrain(500, 500, terrain_model->GetMesh(), new Texture2D("/texture/diffuse.tga"), m_heightMap);
	//m_terrain->SetMaterial(terrainMaterialInstance);
	Add(m_terrain);

	// 보스몬스터 세팅
	Model* model = new Model("/sun/boss_idle.sun");
	Texture2D* texture = new Texture2D("/texture/boss.png");
	m_entity = new Entity(model->GetMesh(), texture);
	//m_entity->SetMaterial(lightMaterialInstance);
	m_entity->GetTransformComponent()->Rotate(-90.f, vec3(1, 0, 0));
	m_entity->GetTransformComponent()->SetScale(vec3(0.3, 0.3, 0.3));

	Add(m_entity);

	// 보스몬스터용 축 그룹 세팅
	m_group = new Group3D();
	m_group->Add(new Entity(MeshFactory::CreateXAxis(), RGBA(1, 0, 0, 1), mat4::Identity()));
	m_group->Add(new Entity(MeshFactory::CreateYAxis(), RGBA(0, 1, 0, 1), mat4::Identity()));
	m_group->Add(new Entity(MeshFactory::CreateZAxis(), RGBA(0, 0, 1, 1), mat4::Identity()));
	Add(m_group);

	// xyz축 세팅
	Entity* xAxis = new Entity(MeshFactory::CreateXAxis(), RGBA(1, 0, 0, 1), mat4::Identity());
	Entity* yAxis = new Entity(MeshFactory::CreateYAxis(), RGBA(0, 1, 0, 1), mat4::Identity());
	Entity* zAxis = new Entity(MeshFactory::CreateZAxis(), RGBA(0, 0, 1, 1), mat4::Identity());
	Add(xAxis);
	Add(yAxis);
	Add(zAxis);

	



	//SetCamera(new FPSCamera(maths::mat4::Perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f)));
}

void BossLayer::OnTick()
{

}

void BossLayer::OnUpdate(const utils::Timestep& ts)
{
	for (auto& c : m_characters)
	{
		c->PlayAnimation();
	}

	m_entity->PlayAnimation();
	
	m_terrain->AdjustPosition(m_entity->GetTransformComponent()->GetPosition());
	m_entity->GetTransformComponent()->SetHeight(m_heightMap->GetHeight(m_entity->GetTransformComponent()->GetPosition()) + 28);
	m_group->GetTransformComponent()->SetPosition(m_entity->GetTransformComponent()->GetPosition());
}

void BossLayer::SetEntity(unsigned int index)
{
	
}

void BossLayer::OnEvent(Event& event)
{
	Layer3D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&BossLayer::OnKeyPressedEvent));
}

bool BossLayer::OnKeyPressedEvent(KeyPressedEvent& event)
{
	if (event.GetKeyCode() == SUNNY_KEY_A)		m_entity->GetTransformComponent()->Translate(vec3(-2, 0, 0));
	if (event.GetKeyCode() == SUNNY_KEY_D)		m_entity->GetTransformComponent()->Translate(vec3(2, 0, 0));
	if (event.GetKeyCode() == SUNNY_KEY_W)		m_entity->GetTransformComponent()->Translate(vec3(0, 0, -2));
	if (event.GetKeyCode() == SUNNY_KEY_S)		m_entity->GetTransformComponent()->Translate(vec3(0, 0, 2));
	
	
	if (event.GetKeyCode() == SUNNY_KEY_1 && !event.GetRepeat()) Renderer3D::DEFERRED_MODE = !Renderer3D::DEFERRED_MODE;
	//std::cout << (m_entity->GetTransformComponent()->GetPosition()) << std::endl;
	
	return false;
}