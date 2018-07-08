#pragma once

#include "../../include.h"
#include "Shaders.h"
#include "../../directx/Shader.h"
#include "ShaderManager.h"

namespace sunny
{
	namespace graphics
	{
		namespace ShaderFactory
		{
			directx::Shader* Default2DShader();
			directx::Shader* Default3DForwardShader();
			directx::Shader* Default3DGeometryShader();
			directx::Shader* Default3DShadowShader();
			directx::Shader* Default3DOutLineShader();
			directx::Shader* DefaultDebugShader();
			directx::Shader* DefaultParticleShader();
		}
	}
}