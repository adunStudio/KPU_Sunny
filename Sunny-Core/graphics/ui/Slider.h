#pragma once

#include "../../sunny.h"
#include "Widget.h"

namespace sunny
{
	namespace graphics
	{
		namespace ui
		{
			class Slider : public Widget
			{
			public:
				typedef std::function<void(float)> ValueChangedCallback;

			private:
				enum class SliderState
				{
					PRESSED, PRESSEDHEAD, UNPRESSED
				};

			private:
				maths::Rectangle m_headBounds;

				float m_value;

				float m_headOffset;

				SliderState m_state;

				ValueChangedCallback m_callback;
			
				bool m_vertical;

			public:
				Slider(const maths::Rectangle bounds, bool vertical = false);
				Slider(const maths::Rectangle bounds, float value = 0.0f, const ValueChangedCallback& callback = &Slider::NoCallback, bool vertical = false);

				virtual bool OnMousePressed (events::MousePressedEvent&  e) override;
				virtual bool OnMouseReleased(events::MouseReleasedEvent& e) override;
				virtual bool OnMouseMoved   (events::MouseMovedEvent&    e) override;
			
				virtual void OnUpdate() override;
				virtual void OnRender(Renderer2D& renderer) override;

				inline void SetCallback(const ValueChangedCallback& callback) { m_callback = callback; }
				inline const ValueChangedCallback& GetCallback() const { return m_callback; }

				inline float GetValue() const { return m_value; }
				void SetValue(float value);

			private:
				static void NoCallback(float) {};
			};
		}
	}
}
