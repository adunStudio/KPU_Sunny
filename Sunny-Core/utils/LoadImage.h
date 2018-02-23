#pragma once

#include "../sunny.h"

namespace sunny
{
	namespace utils
	{
		unsigned char* LoadSunnyImage(const char*  filename, unsigned int* width = nullptr, unsigned int* height = nullptr, unsigned int* bits = nullptr, bool flipY = false);
		unsigned char* LoadSunnyImage(const std::string& filename, unsigned int* width = nullptr, unsigned int* height = nullptr, unsigned int* bits = nullptr, bool flipY = false);
	}
}