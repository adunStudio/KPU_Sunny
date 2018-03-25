#include "HeightMap.h"

#include "../system/VFS.h"

namespace sunny
{
	namespace graphics
	{
		HeightMap::HeightMap(int width, int height, const std::string& path)
		: m_width(width), m_height(height)
		{
			const std::string ext = utils::SplitString(path, ".")[1];

			if (ext != "raw") 
			{
				// Debug System
				std::cout << "raw 파일만 로드 가능합니다." << std::endl;

				exit(1);
			}

			LoadRaw(path);

			//Smooth();
		}

		float HeightMap::GetHeight(const vec3& position)
		{
			return m_map[(position.z + (m_height / 2)) * m_width + position.x];
		}


		void HeightMap::LoadRaw(const std::string& path)
		{
			std::string physicalPath;
			
			if (!VFS::Get()->ResolvePhysicalPath(path, physicalPath))
			{
				// DebugSystem
				std::cout << "존재하지 않는 파일입니다. : " << path << std::endl;
				exit(1);
			}
		
			std::vector<unsigned char> in(m_height * m_width);
		
			std::ifstream file;
			file.open(physicalPath.c_str(), std::ios::binary);

	
			file.read((char*)&in[0], (std::streamsize)in.size());
			
			file.close();

			m_map.resize(m_height * m_width, 0);
			

			for (unsigned int i = 0; i < m_height * m_width; ++i)
				m_map[i] = (in[i] / 255.0f) * 600;
		}

		bool HeightMap::InBounds(int y, int x)
		{
			return
				y >= 0 && y < m_height &&
				x >= 0 && x < m_width;
		}

		float HeightMap::Average(int y, int x)
		{
			float avg = 0.0f;
			float num = 0.0f;

			for (int h = y - 1; h <= y + 1; ++h)
			{
				for (int w = x - 1; x <= x + 1; ++x)
				{
					if (InBounds(h, w))
					{
						avg += m_map[h * m_width + w];
						num += 1.0f;
					}
				}
			}

			return avg / num;
		}

		void HeightMap::Smooth()
		{
			std::vector<float> result(m_map.size());

			for (unsigned int y = 0; y < m_height; ++y)
			{
				for (unsigned int x = 0; x < m_width; ++x)
				{
					result[y * m_width + x] = Average(y, x);
				}
			}

			for (auto a : result)
			{
				std::cout << a << std::endl;
			}

			m_map = result;
		}

	}
}