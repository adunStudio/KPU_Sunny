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

				std::cout << "SDfsdfs " << std::endl;

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

				//shader = directx::Shader::CreateFromSource("default3DForward", sunny::graphics::default3DForwardShader_src);
				shader = directx::Shader::CreateFromFile("default3DForward", "02_HLSL/basic_light.hlsl");
				
				ShaderManager::Add(shader);
				
				return shader;
			};

			directx::Shader* Default3DGeometryShader()
			{
				directx::Shader* shader;
				shader = ShaderManager::Get("default3DGeometry");
				if (shader)
					return shader;

				//shader = directx::Shader::CreateFromSource("default3DDeferred", sunny::graphics::default3DDeferredShader_src);
				shader = directx::Shader::CreateFromFile("default3DDeferred", "02_HLSL/geometry.hlsl");

				ShaderManager::Add(shader);

				return shader;
			}

			directx::Shader* Default3DShadowShader()
			{
				directx::Shader* shader;
				shader = ShaderManager::Get("default3DShadow");
				if (shader)
					return shader;

				shader = directx::Shader::CreateFromFile("default3DShadow", "02_HLSL/shadow.hlsl");

				ShaderManager::Add(shader);

				return shader;
			}
			directx::Shader* Default3DOutLineShader()
			{
				directx::Shader* shader;
				shader = ShaderManager::Get("defaultOutLine");
				if (shader)
					return shader;

				shader = directx::Shader::CreateFromFile("defaultOutLine", "02_HLSL/outline.hlsl");

				ShaderManager::Add(shader);

				return shader;
			}

			directx::Shader* DefaultDebugShader()
			{
				directx::Shader* shader;
				shader = ShaderManager::Get("defaultDebug");
				if (shader)
					return shader;

				shader = directx::Shader::CreateFromFile("defaultDebug", "02_HLSL/debug.hlsl");

				ShaderManager::Add(shader);

				return shader;
			}
		}
	}
}