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

			directx::Shader*Default3DForwardShader()
			{
				directx::Shader* shader;
				shader = ShaderManager::Get("default3DForward");
				if (shader)
					return shader;

				shader = directx::Shader::CreateFromSource("default3DForward", sunny::graphics::default3DForwardShader_src);
				ShaderManager::Add(shader);
				
				return shader;
			};

			directx::Shader* Default3DDeferredShader()
			{
				directx::Shader* shader;
				shader = ShaderManager::Get("default3DDeferred");
				if (shader)
					return shader;

				shader = directx::Shader::CreateFromSource("default3DDeferred", sunny::graphics::default3DDeferredShader_src);
				ShaderManager::Add(shader);

				return shader;
			}

			directx::Shader* Default3DLightShader()
			{
				directx::Shader* shader;
				shader = ShaderManager::Get("default3DLight");
				if (shader)
					return shader;

				shader = directx::Shader::CreateFromSource("default3DLight", sunny::graphics::default3DLightShader_src);
				ShaderManager::Add(shader);

				return shader;
			}
		}
	}
}