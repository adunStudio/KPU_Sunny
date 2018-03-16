#pragma once
#include "../sunny.h"
#include "Widget.h"

namespace sunny
{
	namespace ui
	{
		// 로딩 바, 체력 바 등에 쓰이는 클래스
		class Progressbar : public Widget
		{
		public:
			typedef std::function<void(float)>    CompletedCallback;

		private:
			enum class ProgressbarState
			{
				LOADING, COMPLETED
			};

		private:
			maths::Rectangle m_progressBounds;

			float m_value;

			float m_headOffset;

			ProgressbarState m_state;

			CompletedCallback m_callback;

		public:
			Progressbar(const maths::Rectangle bounds, float value = 0.0f, const CompletedCallback& completedCallback = &Progressbar::NoCallback);

			virtual void OnUpdate() override;
			virtual void OnRender(Renderer2D& renderer) override;

			inline void SetCallback(const CompletedCallback& callback) { m_callback = callback; }
			inline const CompletedCallback& GetCallback() const { return m_callback; }

			inline float GetValue() const { return m_value; }
			void SetValue(float value);

		private:
			static void NoCallback(float) {};
		};
	}
}