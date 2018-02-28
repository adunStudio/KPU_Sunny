#pragma once

#include "../../sunny.h"
#include "../../app/Window.h"
#include "../../utils/Timestep.h"
#include "../../events/Event.h"
#include "../../events/IEventListener.h"

namespace sunny
{
	namespace graphics
	{
		class Layer : public events::IEventListener
		{
		protected:
			Window* m_window;

			bool m_visible;

			bool m_active;

		public:
			Layer();
			virtual ~Layer();

			inline bool IsVisible() const { return m_visible; }
			inline void SetVisible(bool visible) { m_visible = visible; }
			
			inline bool IsActive() const { return m_active; }
			inline void SetActive(bool active) { m_active = active; }

			virtual void Init();
			virtual void OnTick();
			virtual void OnUpdate(const utils::Timestep& ts);
			virtual void OnUpdateInternal(const utils::Timestep& ts);
			virtual void OnRender();

			virtual void OnEvent(events::Event& event);

		protected:
			virtual bool OnResize(unsigned int width, unsigned int height);
		};
	}
}
