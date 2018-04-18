#pragma once

#include "../include.h"
#include "../maths/maths.h"
#include "../system/FileSystem.h"

namespace sunny
{
	namespace graphics
	{
		using namespace maths;

		class HeightMap
		{
		private:
			int m_width, m_height;

			std::vector<float> m_map;

		public:
			HeightMap(int width, int height, const std::string& path);

			float GetHeight(const vec3& position);

		private:
			void LoadRaw(const std::string& path);

			bool InBounds(int y, int x);

			float Average(int y, int x);

			void Smooth();
		};
	}
}