#pragma once

#include <SUNNY.h>
#include "../assets/AssetData.h"

using namespace sunny::graphics;
using namespace sunny::maths;

namespace game
{
	class Model3D : public Entity
	{
	private:
		bool picked;

	public:
		Model3D(const std::string& name, const mat4& transform = mat4::Identity());
	};


	typedef Model3D Tree;
}