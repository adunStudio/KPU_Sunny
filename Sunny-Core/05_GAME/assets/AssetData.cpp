#include "AssetData.h"

namespace game
{
	std::map<std::string, ModelData*> AssetData::s_modelData;


	ModelData* AssetData::GetModelData(const std::string& name)
	{
		return s_modelData[name];
	}

	void AssetData::AddModelData(const std::string& name, ModelData* data)
	{
		s_modelData[name] = data;
	}
}