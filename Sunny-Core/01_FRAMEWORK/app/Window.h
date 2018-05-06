//
// Created by adunstudio on 2018-01-08.
//

#pragma once

#include <tchar.h>
#include <WinSock2.h>
#include <Windows.h>
#include <Windowsx.h>
#include "../include.h"
#include "../events/Events.h"
#include "input.h"
#include "server.h"

#define	WM_SOCKET WM_USER + 1

namespace sunny
{
    struct WindowProperties
    {
        int width, height;
        bool fullscreen;
        bool vsync;
    };

	// std::function<리턴타입(입력 파라미터들)>
	typedef std::function<void(events::Event& event)> WindowEventCallback;

    class Window
    {
    private:
        static std::map<void*, Window*> s_handles;

        static void RegisterWindowClass(void* handle, Window* window);

    private:
        std::string m_title;
        WindowProperties m_properties;
        bool m_closed;
        HWND m_handle;

        WindowEventCallback m_eventCallback;

		InputManager*  m_inputManager;
		ServerManager* m_serverManager;

		// 해상도
		int m_resolutionWidth, m_resolutionHeight;


	public:
		static Window* GetWindowClass(void* handle = nullptr);

    private:
        bool Init();

    public:
        Window(const std::string title, const WindowProperties& properties);
        ~Window();

        void Update();
        void Clear() const;
        bool Closed() const;

        void SetTitle(const std::string title);

        inline float GetWidth()  const { return m_properties.width;  }
        inline float GetHeight() const { return m_properties.height; }

		inline float GetResolutionWidth()  const { return m_resolutionWidth;  }
		inline float GetResolutionHeight() const { return m_resolutionHeight; }

		void         SetVsnyc(bool enabled);
		inline bool  GetVsnyc() const { return m_properties.vsync; }

		inline InputManager*   GetInputManager() const { return m_inputManager;  }
		inline ServerManager* GetServerManager() const { return m_serverManager; }

		inline HWND GetHandle() { return m_handle; }

		void SetEventCallback(const WindowEventCallback& callback);

        friend LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        friend void ResizeCallback(Window* window, int width, int height);
        friend void FocusCallback(Window* window, bool focused);
    };
}

