#include "TextureManager.h"

namespace sunny
{
	namespace graphics
	{
		std::vector<directx::Texture*> TextureManager::m_textures;

		directx::Texture* TextureManager::Add(directx::Texture* texture)
		{
			m_textures.push_back(texture);

			return texture;
		}

		directx::Texture* TextureManager::Get(const std::string& name)
		{
			for (directx::Texture* texture : m_textures)
			{
				if (texture->GetName() == name)
					return texture;
			}

			return nullptr;
		}

		directx::Texture2D* TextureManager::Get2D(const std::string& name)
		{
			for (directx::Texture* texture : m_textures)
			{
				if (texture->GetName() == name)
					return static_cast<directx::Texture2D*>(texture);
			}

			return nullptr;
		}

		void TextureManager::Clean()
		{
			for (unsigned int i = 0; i < m_textures.size(); ++i)
				delete m_textures[i];
		}
	}
}