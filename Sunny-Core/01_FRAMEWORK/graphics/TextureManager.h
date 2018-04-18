#pragma once

#include "../directx/Texture.h"

namespace sunny
{
	namespace graphics
	{
		class TextureManager
		{
		private:
			static std::vector<directx::Texture*> m_textures;

		public:
			static directx::Texture* Add(directx::Texture* texture);
			static directx::Texture* Get(const std::string& name);

			static void Clean();

		private:
			TextureManager() {};
		};
	}
}