#pragma once

#include "../../sunny.h"
#include "../renderables/Renderable3D.h"

namespace sunny
{
	namespace graphics
	{
		class Group3D : public Renderable3D
		{
		private:
			std::vector<Renderable3D*> m_renderables;

		public:
			Group3D(const maths::mat4& transform = maths::mat4::Identity());
			~Group3D();

			std::vector<Renderable3D*>& GetRenderableList() { return m_renderables; }

			void Add(Renderable3D* renderable);

			virtual void Render() override;
		};
	}
}