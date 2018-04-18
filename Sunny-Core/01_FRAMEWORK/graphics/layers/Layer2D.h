#pragma once

#include "../../include.h"

#include "Layer.h"
#include "../renderers/Renderer2D.h"
#include "../renderables/Renderable2D.h"
#include "../Mask.h"
#include "../Sprite.h"
#include "../Label.h"
#include "../../utils/Timestep.h"
#include "../cameras/OrthographicCamera.h"

typedef sunny::maths::vec4 RGBA;

namespace sunny
{
	namespace graphics
	{
		class Layer2D : public Layer
		{
		private:
			Renderer2D* m_renderer;
			OrthographicCamera* m_camera;

		protected:
			std::vector<Renderable2D*> m_renderables;

		public:
			Layer2D(const maths::mat4& projectionMatrix);
			virtual ~Layer2D();

			virtual Renderable2D* Add(Renderable2D* renderable);

			virtual void Init();
			virtual void OnInit(Renderer2D& renderer);

			inline void SetMask(const Mask* mask) const { m_renderer->SetMask(mask); }

			void OnUpdateInternal(const utils::Timestep& ts) override;

			virtual void OnRender(Renderer2D& renderer);
			void OnRender() override;

		protected:
			bool OnResize(unsigned int whdith, unsigned int height) override;
		};
	}
}