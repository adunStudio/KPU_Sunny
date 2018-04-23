#include "Model3D.h"

namespace game
{
	Model3D::Model3D(const std::string& name, const mat4& transform)
	: Entity(transform), picked(false)
	{
		ModelData* data = AssetData::GetModelData(name);
		
		if (!data)
		{
			// Debug System
			std::cout << "Not ModelData: " << name << std::endl;
			exit(1);
		}

		m_mesh = ModelManager::GetMesh(name);
		
		for (int i = 0; i < data->textures.size(); ++i)
		{
			m_textures.push_back(TextureManager::Get(data->textures[i]));
		}
	}

	void Model3D::Update()
	{
		if (!picked) return;
	
		//GetTransformComponent()->Translate(vec3(1, 1, 0));
	}

}