//
// Created by adunstudio on 2018-01-10.
//

#include "Application.h"
#include "../directx/Context.h"

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

	void Application::PushLayer2D(graphics::Layer* layer)
	{
		m_layer2DStack.push_back(layer);

		layer->Init();
	}

	void Application::PushLayer3D(graphics::Layer* layer)
	{
		m_layer3DStack.push_back(layer);

		layer->Init();
	}

	graphics::Layer* Application::PopLayer2D()
	{
		graphics::Layer* layer = m_layer2DStack.back();
		m_layer2DStack.pop_back();

		return layer;
	}

	graphics::Layer* Application::PopLayer3D()
	{
		graphics::Layer* layer = m_layer3DStack.back();
		m_layer3DStack.pop_back();

		return layer;
	}

	graphics::Layer* Application::PopLayer(graphics::Layer* layer)
	{
		for (unsigned int i = 0; i < m_layer2DStack.size(); ++i)
		{
			if (m_layer2DStack[i] == layer)
			{
				m_layer2DStack.erase(m_layer2DStack.begin() + i);
			}
		}

		for (unsigned int i = 0; i < m_layer3DStack.size(); ++i)
		{
			if (m_layer3DStack[i] == layer)
			{
				m_layer3DStack.erase(m_layer3DStack.begin() + i);
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
		}
    }

    void Application::OnTick()
    {
		for (unsigned int i = 0; i < m_overlayStack.size(); ++i)
			if(m_overlayStack[i]->IsActive())
				m_overlayStack[i]->OnTick();
		
		for (unsigned int i = 0; i < m_layer3DStack.size(); ++i)
			if (m_layer3DStack[i]->IsActive())
				m_layer3DStack[i]->OnTick();

		for (unsigned int i = 0; i < m_layer2DStack.size(); ++i)
			if (m_layer2DStack[i]->IsActive())
				m_layer2DStack[i]->OnTick();
	}

    void Application::OnUpdate(const utils::Timestep& ts)
    {
		for (unsigned int i = 0; i < m_overlayStack.size(); ++i)
			if (m_overlayStack[i]->IsActive())
				m_overlayStack[i]->OnUpdateInternal(ts);

		for (unsigned int i = 0; i < m_layer3DStack.size(); ++i)
			if (m_layer3DStack[i]->IsActive())
				m_layer3DStack[i]->OnUpdateInternal(ts);

		for (unsigned int i = 0; i < m_layer2DStack.size(); ++i)
			if (m_layer2DStack[i]->IsActive())
				m_layer2DStack[i]->OnUpdateInternal(ts);
    }

    void Application::OnRender()
    {
		for (unsigned int i = 0; i < m_layer3DStack.size(); ++i)
			if (m_layer3DStack[i]->IsVisible())
				m_layer3DStack[i]->OnRender();

		if (m_layer3DStack.size() >= 1)
			directx::Context::GetContext()->Present3D();

		for (unsigned int i = 0; i < m_layer2DStack.size(); ++i)
			if (m_layer2DStack[i]->IsVisible())
				m_layer2DStack[i]->OnRender();

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

		for (int i = m_layer3DStack.size() - 1; i >= 0; --i)
		{
			if (m_layer3DStack[i]->IsActive())
				m_layer3DStack[i]->OnEvent(event);

			if (event.isHandled()) return;
		}

		for (int i = m_layer2DStack.size() - 1; i >= 0; --i)
		{
			if (m_layer2DStack[i]->IsActive())
				m_layer2DStack[i]->OnEvent(event);

			if (event.isHandled()) return;
		}
	}
}
