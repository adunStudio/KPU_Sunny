#pragma once

#include "../../sunny.h"
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
			directx::Shader* Default3DDeferredShader();
			directx::Shader* Default3DLightShader();
		}
	}
}