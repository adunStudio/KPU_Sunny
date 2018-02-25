#pragma once

#include "../../sunny.h"
#include "Layer.h"
#include "../renderers/Renderer3D.h"
#include "../cameras/camera.h"
#include "../renderables/Renderable3D.h"
#include "../Model.h"
#include "../Entity.h"

#include "../cameras/MayaCamera.h"
#include "../cameras/FPSCamera.h"

typedef sunny::maths::vec4 RGBA;

namespace sunny
{
	namespace graphics
	{
		class Layer3D : public Layer
		{
		private:
			Camera* m_camera;

			std::vector<Renderable3D*> m_renderables;

		protected:
			Renderer3D* m_renderer;

		public:
			Layer3D(Renderer3D * renderer = new Renderer3D());
			virtual ~Layer3D();

			virtual Renderable3D* Add(Renderable3D* renderable);

			void SetCamera(Camera * camera);

			virtual void Init();
			virtual void OnInit(Renderer3D& renderer);

			void OnUpdateInternal(const utils::Timestep& ts) override;

			virtual void OnRender(Renderer3D& renderer);
			void OnRender() override;

		protected:
			bool OnResize(unsigned int whdith, unsigned int height) override;
		};
	}
}