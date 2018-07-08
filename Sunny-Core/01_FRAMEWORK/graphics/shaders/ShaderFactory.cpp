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
				shader = directx::Shader::CreateFromFile("default3DForward", "02_HLSL/01_forward.hlsl");
				
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
				shader = directx::Shader::CreateFromFile("default3DDeferred", "02_HLSL/04_geometry.hlsl");

				ShaderManager::Add(shader);

				return shader;
			}

			directx::Shader* Default3DShadowShader()
			{
				directx::Shader* shader;
				shader = ShaderManager::Get("default3DShadow");
				if (shader)
					return shader;

				shader = directx::Shader::CreateFromFile("default3DShadow", "02_HLSL/02_shadow.hlsl");

				ShaderManager::Add(shader);

				return shader;
			}
			directx::Shader* Default3DOutLineShader()
			{
				directx::Shader* shader;
				shader = ShaderManager::Get("defaultOutLine");
				if (shader)
					return shader;

				shader = directx::Shader::CreateFromFile("defaultOutLine", "02_HLSL/05_outline.hlsl");

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

			directx::Shader* DefaultParticleShader()
			{
				directx::Shader* shader;
				shader = ShaderManager::Get("defaultParticle");
				if (shader)
					return shader;

				shader = directx::Shader::CreateFromFile("defaultParticle", "02_HLSL/06_particle.hlsl");

				ShaderManager::Add(shader);

				return shader;
			}
		}
	}
}