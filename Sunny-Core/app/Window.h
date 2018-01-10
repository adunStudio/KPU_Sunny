//
// Created by adunstudio on 2018-01-08.
//

#pragma once

#include <tchar.h>
#include <Windows.h>
#include "../sunny.h"

namespace sunny
{
    namespace app
    {

        struct WindowProperties
        {
            int width, height;
            bool fullscreen;
            bool vsync;
        };

        class Window
        {
        private:
            static std::map<void*, Window*> s_handles;

            static void RegisterWindowClass(void* handle, Window* window);
            static Window* GetWindowClass(void* handle);

        private:
            std::string m_title;
            WindowProperties m_properties;
            bool m_closed;
            void* m_handle;

        private:
            bool Init();

        public:
            Window(const std::string title, const WindowProperties& properties);
            ~Window();

            void Run();
            void Clear() const;
            bool Closed() const;

            void SetTitle(const std::string title);


            friend LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
            friend void ResizeCallback(Window* window, int width, int height);
            friend void FocusCallback(Window* window, bool focused);
        };
    }
}

