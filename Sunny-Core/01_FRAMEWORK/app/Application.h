//
// Created by adunstudio on 2018-01-10.
//

#pragma once

#include <mutex>
#include "../include.h"
#include "Window.h"
#include "../utils/Timer.h"
#include "../utils/Timestep.h"
#include "../maths/vec2.h"
#include "../graphics/layers/Layer.h"
#include "../system/VFS.h"
#include "../events/Events.h"
namespace sunny
{
    class Application
    {
    public:
        Window* window;

		std::mutex m_mutex;

        inline static Application& GetApplication() { return *s_instance; }
    private:
        static Application* s_instance;

    private:
        bool m_running, m_paused;
        utils::Timer* m_timer;

        int m_updatesPerSecond, m_framesPerSecond;
        float m_frameTime;

        std::string m_name;
        WindowProperties m_properties;

		std::vector<graphics::Layer*> m_layerStack;
		std::vector<graphics::Layer*> m_overlayStack;

    public:
        Application(const std::string& name, const WindowProperties& properties);
        ~Application();

        virtual void Init();

        void Start();
        void Pause();
        void Resume();
        void Stop();

		void PushLayer(graphics::Layer* layer);
		graphics::Layer* PopLayer();
		graphics::Layer* PopLayer(graphics::Layer* layer);

		void PushOverlay(graphics::Layer* layer);
		graphics::Layer* PopOverlay();
		graphics::Layer* PopOverlay(graphics::Layer* layer);

        inline int GetFPS() const { return m_framesPerSecond;   }
        inline int GetTPS() const { return m_updatesPerSecond;  }
        inline float GetFrameTime() const { return m_frameTime; }

        inline int GetWindowWidth()  const { return window->GetWidth();  }
        inline int GetWindowHeight() const { return window->GetHeight(); }
        inline maths::vec2 GetWindowSize() const
        {
            return maths::vec2(static_cast<float>(window->GetWidth()), static_cast<float>(window->GetHeight()));
        }

    private:
        void Run();

        void OnTick();
        void OnUpdate(const utils::Timestep& ts);
        void OnRender();

		void OnEvent(events::Event& event);
    };
}
