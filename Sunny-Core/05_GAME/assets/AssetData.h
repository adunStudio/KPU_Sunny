#pragma once

#include <SUNNY.h>

namespace game
{
	struct ModelData
	{
		std::vector<std::string> textures;;
	};


	class AssetData
	{
	private:
		AssetData() = delete;

		static std::map<std::string, ModelData*> s_modelData;
 

	public:
		static ModelData* GetModelData(const std::string& name);
		static void AddModelData(const std::string& name, ModelData* data);
	};
}