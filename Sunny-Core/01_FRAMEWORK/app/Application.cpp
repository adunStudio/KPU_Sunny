//
// Created by adunstudio on 2018-01-10.
//

#include "Application.h"

namespace sunny
{
    Application* Application::s_instance = nullptr;

    Application::Application(const std::string& name, const WindowProperties& properties)
    : m_name(name), m_properties(properties), m_running(false), m_paused(false)
    {
        s_instance = this;
    }
	
    Application::~Application()
    {
        delete window;
    }

    void Application::Init()
    {
		VFS::Init();
        window = new Window(m_name, m_properties);
		window->SetEventCallback(METHOD(&Application::OnEvent));
    }

    void Application::Start()
    {
        Init();

        m_running = true;
        m_paused = false;

        Run();
    }

    void Application::Pause()
    {
        m_paused = true;
    }

    void Application::Resume()
    {
        m_paused = false;
    }

    void Application::Stop()
    {
        m_running = true;
    }

	void Application::PushLayer(graphics::Layer* layer)
	{
		m_layerStack.push_back(layer);


		layer->Init();

	}

	graphics::Layer* Application::PopLayer()
	{
		graphics::Layer* layer = m_layerStack.back();
		m_layerStack.pop_back();

		return layer;
	}

	graphics::Layer* Application::PopLayer(graphics::Layer* layer)
	{
		for (unsigned int i = 0; i < m_layerStack.size(); ++i)
		{
			if (m_layerStack[i] == layer)
			{
				m_layerStack.erase(m_layerStack.begin() + i);
			}
		}

		return layer;
	}

	void Application::PushOverlay(graphics::Layer* layer)
	{
		m_overlayStack.push_back(layer);


		layer->Init();

	}

	graphics::Layer* Application::PopOverlay()
	{
		graphics::Layer* layer = m_overlayStack.back();
		m_overlayStack.pop_back();

		return layer;
	}

	graphics::Layer* Application::PopOverlay(graphics::Layer* layer)
	{
		for (unsigned int i = 0; i < m_overlayStack.size(); ++i)
		{
			if (m_overlayStack[i] == layer)
			{
				m_overlayStack.erase(m_overlayStack.begin() + i);
			}
		}

		return layer;
	}

    void Application::Run()
    {
        m_timer = new utils::Timer();

        float timer = 0.0f;
        float updateTimer = m_timer->ElapsedMillis();
        float updateTick = 1000.0f / 60.0f;

        int frames = 0;
        int updates = 0;

        utils::Timestep timestep(m_timer->ElapsedMillis());

        while(m_running)
        {
			m_mutex.lock();

            window->Clear();

            float now = m_timer->ElapsedMillis();


            // updateTick(1000 / 60)마다 실행
            if(now - updateTimer > updateTick)
            {
                timestep.Update(now);
                OnUpdate(timestep);

                updates++;
                updateTimer += updateTick;
            }


            { // 가능한한 빠르게
                utils::Timer frametime;

				OnRender();

                frames++;
                m_frameTime = frametime.ElapsedMillis();
            }


            window->Update();

            // 1초마다 실행
            if(m_timer->Elapsed() - timer > 1.0f)
            {
                timer += 1.0f;
                m_framesPerSecond = frames;
                m_updatesPerSecond = updates;
                frames = 0;
                updates = 0;
              
				OnTick();
            }

            if(window->Closed())
                m_running = false;

			m_mutex.unlock();

		}
    }

    void Application::OnTick()
    {
		for (unsigned int i = 0; i < m_overlayStack.size(); ++i)
			if(m_overlayStack[i]->IsActive())
				m_overlayStack[i]->OnTick();
		
		for (unsigned int i = 0; i < m_layerStack.size(); ++i)
			if (m_layerStack[i]->IsActive())
				m_layerStack[i]->OnTick();
	}

    void Application::OnUpdate(const utils::Timestep& ts)
    {
		for (unsigned int i = 0; i < m_overlayStack.size(); ++i)
			if (m_overlayStack[i]->IsActive())
				m_overlayStack[i]->OnUpdateInternal(ts);

		for (unsigned int i = 0; i < m_layerStack.size(); ++i)
			if (m_layerStack[i]->IsActive())
				m_layerStack[i]->OnUpdateInternal(ts);
    }

    void Application::OnRender()
    {
		for (unsigned int i = 0; i < m_layerStack.size(); ++i)
			if (m_layerStack[i]->IsVisible())
				m_layerStack[i]->OnRender();

		for (unsigned int i = 0; i < m_overlayStack.size(); ++i)
			if (m_overlayStack[i]->IsVisible())
				m_overlayStack[i]->OnRender();
    }

	void Application::OnEvent(events::Event& event)
	{
		if (event.isHandled()) return;

		// 거꾸로
		for (int i = m_overlayStack.size() - 1; i >= 0; --i)
		{
			if (m_overlayStack[i]->IsActive())
				m_overlayStack[i]->OnEvent(event);

			if (event.isHandled()) return;
		}

		for (int i = m_layerStack.size() - 1; i >= 0; --i)
		{
			if (m_layerStack[i]->IsActive())
				m_layerStack[i]->OnEvent(event);

			if (event.isHandled()) return;
		}
	}
}
