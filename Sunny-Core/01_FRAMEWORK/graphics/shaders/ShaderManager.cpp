#include "ShaderManager.h"

namespace sunny
{
	namespace graphics
	{
		std::vector<directx::Shader*> ShaderManager::s_shaders;

		void ShaderManager::Add(directx::Shader* shader)
		{
			s_shaders.push_back(shader);
		}

		directx::Shader* ShaderManager::Get(const std::string& name)
		{
			for (directx::Shader* shader : s_shaders)
			{
				if (shader->GetName() == name)
					return shader;
			}

			return nullptr;
		}

		void ShaderManager::Clean()
		{
			for (unsigned int i = 0; i < s_shaders.size(); ++i)
				delete s_shaders[i];
		}

		void ShaderManager::Reload(const std::string& name)
		{
			for (unsigned int i = 0; i < s_shaders.size(); ++i)
			{
				if (s_shaders[i]->GetName() == name)
				{
					std::string path = s_shaders[i]->GetFilePath();
					std::string error;

					if (!directx::Shader::TryCompileFromFile(path, error))
					{
						//std::cout << error << std::endl;
						std::cout << "ShaderManager::Reload" << std::endl;
						// Debug System
						//exit(0);
					}
					else
					{
						s_shaders[i]->~Shader();
						s_shaders[i] = directx::Shader::CreateFromFile(name, path, s_shaders[i]);
					}
					return;
				}	
			}

			std::cout << "리로드하기 위한 " << name << " 을 찾을 수 없습니다." << std::endl;
		}

		void ShaderManager::Reload(const directx::Shader* shader)
		{
			for (unsigned int i = 0; i < s_shaders.size(); ++i)
			{
				if (s_shaders[i] == shader)
				{
					std::string path = shader->GetFilePath();
					std::string name = shader->GetName();

					s_shaders[i]->~Shader();
					s_shaders[i] = directx::Shader::CreateFromFile(name, path, s_shaders[i]);
					return;
				}
			}

			std::cout << "리로드하기 위한 셰이더를 찾을 수 없습니다." << std::endl;
		}
	}
}