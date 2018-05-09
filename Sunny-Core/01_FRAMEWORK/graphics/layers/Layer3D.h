#pragma once

#include "../../include.h"
#include "Layer.h"
#include "../renderers/Renderer3D.h"
#include "../cameras/camera.h"
#include "../renderables/Renderable3D.h"
#include "../groups/Group3D.h"
#include "../Model.h"
#include "../Entity.h"
#include "../Terrain.h"
#include "../HeightMap.h"
#include "../LightSetup.h"
#include "../../component/Components.h"
#include "../cameras/MayaCamera.h"
#include "../cameras/FPSCamera.h"
#include "../cameras/LightCamera.h"
#include "../cameras/QuaterCamera.h"


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
			std::vector<Group3D*> m_group3ds;

			std::vector<LightSetup*> m_lightSetupStack;

		protected:
			Renderer3D* m_renderer;

		public:
			Layer3D(Renderer3D * renderer = new Renderer3D());
			virtual ~Layer3D();

			virtual Renderable3D* Add(Renderable3D* renderable);
			virtual Renderable3D* AddStatic(Renderable3D* renderable);

			virtual Group3D* Add(Group3D* groupe3d);

			void SetCamera(Camera * camera);
			inline Camera* GetCamera() { return m_camera; }

			virtual void Init();
			virtual void OnInit(Renderer3D& renderer);

			void OnUpdateInternal(const utils::Timestep& ts) override;

			virtual void OnRender(Renderer3D& renderer);
			void OnRender() override;

			void PushLightSetup(LightSetup* lightSetup);
			LightSetup* PopLightSetup();

			void SetSkybox(Renderable3D* renderable);

		protected:
			bool OnResize(unsigned int whdith, unsigned int height) override;
		};
	}
}