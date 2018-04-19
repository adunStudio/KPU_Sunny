#pragma once

#include "../include.h"
#include "Model.h"

namespace sunny
{
	namespace graphics
	{
		class ModelManager
		{
		private:
			static std::map<std::string, Model*> s_map;

			ModelManager() = delete;

		public:

			static Model* Add(const::std::string& name, Model* model);

			static void Clean();

			static Model* Get(const std::string& name);

			static Mesh* GetMesh(const std::string& name);
		};
	}
}