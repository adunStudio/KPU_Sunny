#include "Layer.h"

namespace sunny
{
	namespace graphics
	{
		Layer::Layer() : m_window(Window::GetWindowClass(nullptr)), m_visible(true)
		{

		}

		Layer::~Layer()
		{

		}

		void Layer::Init()
		{

		}

		void Layer::OnTick()
		{

		}

		void Layer::OnUpdate(const utils::Timestep& ts)
		{

		}
		
		void Layer::OnUpdateInternal(const utils::Timestep& ts)
		{
			
		}
		
		void Layer::OnRender()
		{

		}

		void Layer::OnEvent(events::Event& event)
		{
			events::EventDispatcher dispatcher(event);
			dispatcher.Dispatch <events::ResizeEvent>([this](events::ResizeEvent& e) { return OnResize(e.GetWidth(), e.GetHeight()); });
		}

		bool Layer::OnResize(unsigned int width, unsigned int height)
		{
			return false;
		}
	}
}