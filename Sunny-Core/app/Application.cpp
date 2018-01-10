//
// Created by adunstudio on 2018-01-10.
//

#include "Application.h"

namespace sunny
{
    namespace app
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
            window = new Window(m_name, m_properties);
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
            std::cout <<"UPS: " << GetUPS() << std::endl;
            std::cout <<"FPS: " << GetFPS() << std::endl;
        }

        void Application::OnUpdate(const utils::Timestep& ts)
        {
            
        }

        void Application::OnRender()
        {
            //std::cout << "render" << std::endl;
        }
    }
}
