#pragma once

#include "../../sunny.h"
#include "../../app/Window.h"
#include "../../utils/Timestep.h"

namespace sunny
{
	namespace graphics
	{
		class Layer
		{
		protected:
			Window* m_window;

			bool m_visible;

		public:
			Layer();
			virtual ~Layer();

			inline bool IsVisible() const { return m_visible; }
			inline bool SetVisible(bool visible) { m_visible = visible; }
		
			virtual void Init();
			virtual void OnTick();
			virtual void OnUpdate(const utils::Timestep& ts);
			virtual void OnUpdateInternal(const utils::Timestep& ts);
			virtual void OnRender();
		};
	}
}
