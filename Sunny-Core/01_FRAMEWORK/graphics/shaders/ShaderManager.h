#pragma once

#include "../../include.h"
#include "../../directx/Shader.h"

namespace sunny
{
	namespace graphics
	{
		class ShaderManager
		{
		private:
			static std::vector<directx::Shader*> s_shaders;

		public:
			static void Add(directx::Shader* shader);

			static directx::Shader* Get(const std::string& name);

			static void Clean();

			static void Reload(const std::string& name);
			static void Reload(const directx::Shader* shader);

		private:
			ShaderManager() { }
		};
	}
}