#pragma once

#include "../../include.h"
#include "Mesh.h"
#include "../../maths/maths.h"
#include "../../directx/VertexArray.h"
#include "../../directx/Texture2D.h"

namespace sunny
{
	namespace graphics
	{
		namespace MeshFactory
		{
			Mesh* CreateCube(float size);

			Mesh* CreateQuad(float x, float y, float width, float height);

			Mesh* CreatePlane(float width, float height, const maths::vec3& normal);

			Mesh* CreateXAxis();
			Mesh* CreateYAxis();
			Mesh* CreateZAxis();
		}
	}
}
