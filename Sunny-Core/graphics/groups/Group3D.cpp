#include "Group3D.h"

namespace sunny
{
	namespace graphics
	{
		Group3D::Group3D(const maths::mat4& transform)
		: Renderable3D(transform)
		{

		}
		
		Group3D::~Group3D()
		{
			for (int i = 0; i < m_renderables.size(); ++i)
				delete m_renderables[i];
		}

		void Group3D::Add(Renderable3D* renderable)
		{
			m_renderables.push_back(renderable);
		}

		void Group3D::Render()
		{
			for (Renderable3D* renderable : m_renderables)
			{
				renderable->Render();
			}
		}
	}
}