#include "ShaderFactory.h"

namespace sunny
{
	namespace graphics
	{
		namespace ShaderFactory
		{
			directx::Shader* Default2DShader()
			{
				directx::Shader* shader;
				shader = ShaderManager::Get("default2D");
				if (shader)
					return shader;

				shader = directx::Shader::CreateFromSource("default2D", sunny::graphics::default2DShader_src);

				ShaderManager::Add(shader);

				return shader;
				//return directx::Shader::CreateFromSource("default2D", sunny::graphics::default2DShader_src);
			};

			directx::Shader* Default3DShader()
			{
				directx::Shader* shader;
				shader = ShaderManager::Get("default3D");
				if (shader)
					return shader;

				shader = directx::Shader::CreateFromSource("default3D", sunny::graphics::default3DShader_src);
				ShaderManager::Add(shader);
				
				return shader;
			};
		}
	}
}