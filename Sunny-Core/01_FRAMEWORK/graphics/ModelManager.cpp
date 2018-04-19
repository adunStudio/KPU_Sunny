#include "ModelManager.h"

namespace sunny
{
	namespace graphics
	{
		std::map<std::string, Model*> ModelManager::s_map;

		Model* ModelManager::Add(const::std::string& name, Model* Model)
		{
			s_map[name] = Model;
			
			return Model;
		}

		void ModelManager::Clean()
		{
			for (auto it = s_map.begin(); it != s_map.end(); ++it)
			{
				if (it->second)
				{
					delete it->second;
				}
			}
		}

		Model* ModelManager::Get(const std::string& name)
		{
			return s_map[name];
		}

		Mesh* ModelManager::GetMesh(const std::string& name)
		{
			if (s_map[name] == nullptr) return nullptr;
			
			return s_map[name]->GetMesh();
		}
	}
}