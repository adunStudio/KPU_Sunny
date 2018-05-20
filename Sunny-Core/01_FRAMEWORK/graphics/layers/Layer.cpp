#include "Layer.h"
#include "../../app/Application.h"

namespace sunny
{
	namespace graphics
	{
		Layer::Layer() : m_window(Window::GetWindowClass(nullptr)), m_visible(true), m_active(true), m_windowWidth(Application::GetApplication().GetWindowWidth()), m_windowHeight(Application::GetApplication().GetWindowHeight())
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