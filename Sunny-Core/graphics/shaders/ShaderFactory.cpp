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

				//shader = directx::Shader::CreateFromSource("default3DForward", sunny::graphics::default3DForwardShader_src);
				shader = directx::Shader::CreateFromFile("default3DForward", "resource/hlsl/basic_light.hlsl");
				
				ShaderManager::Add(shader);
				
				return shader;
			};

			directx::Shader* Default3DDeferredShader()
			{
				directx::Shader* shader;
				shader = ShaderManager::Get("default3DDeferred");
				if (shader)
					return shader;

				//shader = directx::Shader::CreateFromSource("default3DDeferred", sunny::graphics::default3DDeferredShader_src);
				shader = directx::Shader::CreateFromFile("default3DDeferred", "resource/hlsl/geometry.hlsl");

				ShaderManager::Add(shader);

				return shader;
			}

			directx::Shader* Default3DLightShader()
			{
				directx::Shader* shader;
				shader = ShaderManager::Get("default3DLight");
				if (shader)
					return shader;

				//shader = directx::Shader::CreateFromSource("default3DLight", sunny::graphics::default3DLightShader_src);
				shader = directx::Shader::CreateFromFile("default3DLight", "resource/hlsl/deferred.hlsl");

				ShaderManager::Add(shader);

				return shader;
			}

			directx::Shader* Default3DShadowShader()
			{
				directx::Shader* shader;
				shader = ShaderManager::Get("default3DShadow");
				if (shader)
					return shader;

				shader = directx::Shader::CreateFromFile("default3DShadow", "resource/hlsl/shadow.hlsl");

				ShaderManager::Add(shader);

				return shader;
			}
			directx::Shader* Default3DOutLineShader()
			{
				directx::Shader* shader;
				shader = ShaderManager::Get("defaultOutLine");
				if (shader)
					return shader;

				shader = directx::Shader::CreateFromFile("defaultOutLine", "resource/hlsl/outline.hlsl");

				ShaderManager::Add(shader);

				return shader;
			}

			directx::Shader* DefaultDebugShader()
			{
				directx::Shader* shader;
				shader = ShaderManager::Get("defaultDebug");
				if (shader)
					return shader;

				shader = directx::Shader::CreateFromFile("defaultDebug", "resource/hlsl/debug.hlsl");

				ShaderManager::Add(shader);

				return shader;
			}
		}
	}
}