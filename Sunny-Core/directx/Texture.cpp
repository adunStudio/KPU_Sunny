#include "Texture.h"

namespace sunny
{
	namespace directx
	{
		unsigned char Texture::GetStrideFromFormat(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::RGB:  return 3;
			case TextureFormat::RGBA: return 4;
			}

			return 0;
		}
	}
}