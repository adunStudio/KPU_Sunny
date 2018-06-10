#pragma once

#include <SUNNY.h>
#include "../assets/AssetData.h"

using namespace sunny::graphics;
using namespace sunny::maths;

namespace game
{
	class Model3D : public Entity
	{
	public:
		bool picked;
		std::string name;

	private:
		maths::vec2 m_initialMousePosition;

	public:
		Model3D(const std::string& name, const mat4& transform = mat4::Identity());

		void Update();
	};


	typedef Model3D Tree;
}