#pragma once

#include "../sunny.h"
#include "Mesh.h"
#include "../maths/maths.h"
#include "../directx/VertexArray.h"
#include "../directx/Texture2D.h"

namespace sunny
{
	namespace graphics
	{
		namespace MeshFactory
		{
			Mesh* CreateCube(float size);
		}
	}
}
