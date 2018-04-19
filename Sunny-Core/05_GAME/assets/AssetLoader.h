#pragma once

#include <SUNNY.h>
#include "AssetData.h"

namespace game
{
	class AssetLoader
	{
	private:
		AssetLoader() = delete;

	public:

		static void LoadModel(const std::string& jsonPath);
	};
}
