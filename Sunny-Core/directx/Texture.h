#pragma once

#include "../sunny.h"
#include "../utils/LoadImage.h"

namespace sunny
{
	enum class TextureFormat
	{
		NONE = 0,
		RGB,
		RGBA,
	};

	struct TextureParameters
	{
		TextureFormat format;

		TextureParameters() : format(TextureFormat::RGBA) { }

		TextureParameters(TextureFormat format) : format(format) { }
	};

	struct TextureLoadOptions
	{
		bool flipX;
		bool flipY;

		TextureLoadOptions() : flipX(false), flipY(false) { }
		TextureLoadOptions(bool flipX, bool flipY) : flipX(flipX), flipY(flipY) { }
	};

	namespace directx
	{
		
		class Texture
		{

		public:
			virtual ~Texture() {};

			virtual void Bind(unsigned int slot = 0) const = 0;
			virtual void UnBind(unsigned int slot = 0) const = 0;

			virtual const std::string& GetName() const = 0;
			virtual const std::string& GetFilePath() const = 0;

		public:

			static unsigned char GetStrideFromFormat(TextureFormat format);
		};
	}
}