#include "Input.h"

#include <Windows.h>

#include "../app/Window.h"
#include "../events/Events.h"

namespace sunny
{
	extern HWND hWnd; // Window.cpp

	InputManager* Input::s_inputManager = nullptr;

	InputManager::InputManager()
	{
		ClearKeys();
		ClearMouseButtons();

		m_mouseGrabbed = true;

		Input::s_inputManager = this;
	}

	void InputManager::Update()
	{
		for (unsigned int i = 0; i < MAX_BUTTONS; ++i)
		{
			m_mouseClicked[i] = m_mouseButtons[i] && !m_lastMouseState[i];
		}

		memcpy(m_lastKeyState, m_keyState, MAX_KEYS);
		memcpy(m_lastMouseState, m_mouseButtons, MAX_BUTTONS);

		// PlatformUpdate
		POINT mouse;
		GetCursorPos(&mouse);
		ScreenToClient(hWnd, &mouse);

		maths::vec2 mousePos = maths::vec2((float)mouse.x, (float)mouse.y);
		
		if (mousePos != m_mousePosition)
		{
			m_eventCallback(events::MouseMovedEvent(mousePos.x, mousePos.y, m_mouseButtons[SUNNY_MOUSE_LEFT]));
			m_mousePosition = mousePos;
		}
	}

	void InputManager::ClearKeys()
	{
		for (unsigned int i = 0; i < MAX_KEYS; ++i)
		{
			m_keyState[i]     = false;
			m_lastKeyState[i] = false;
		}

		m_keyModifiers = 0;
	}

	void InputManager::ClearMouseButtons()
	{
		for (unsigned int i = 0; i < MAX_BUTTONS; ++i)
		{
			m_mouseButtons[i]     = false;
			m_lastMouseState[i]   = false;
			m_mouseClicked[i]     = false;
		}
	}

	bool InputManager::IsKeyPressed(unsigned int keycode) const
	{
		if (keycode >= MAX_KEYS)
			return false;

		return m_keyState[keycode];
	}

	bool InputManager::IsMouseButtonPressed(unsigned int button) const
	{
		if (button >= MAX_BUTTONS)
			return false;

		return m_mouseButtons[button];
	}

	bool InputManager::IsMouseButtonClicked(unsigned int button) const
	{
		if (button > MAX_BUTTONS)
			return false;

		return m_mouseClicked[button];
	}

	const maths::vec2& InputManager::GetMousePosition() const
	{
		return m_mousePosition;
	}

	void InputManager::SetMousePosition(const maths::vec2& position)
	{
		POINT pt = { (LONG)position.x, (LONG)position.y };
		ClientToScreen(hWnd, &pt);
		SetCursorPos(pt.x, pt.y);
	}

	void InputManager::SetMouseGrabbed(bool grabbed)
	{
		m_mouseGrabbed = grabbed;
	}

	const bool InputManager::IsMouseGrabbed() const
	{
		return m_mouseGrabbed;
	}

	// Ä¿¼­
	void InputManager::SetMouseCursor(int cursor)
	{
		if (cursor == SUNNY_NO_CURSOR)
		{
			SetCursor(SUNNY_NO_CURSOR);
			while (ShowCursor(false) >= 0);
		}
		else
		{
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			ShowCursor(true);
		}
	}

	void KeyCallback(InputManager* inputManager, int flags, int key, unsigned int message)
	{
		bool pressed = (message == WM_KEYDOWN || message == WM_SYSKEYDOWN);
		inputManager->m_keyState[key] = pressed;

		bool repeat = (flags >> 30) & 1;

		int modifier = 0;

		// ctrl, alt, shift
		switch (key)
		{
		case SUNNY_KEY_CONTROL:
			modifier = SUNNY_MODIFIER_LEFT_CONTROL;
			break;

		case SUNNY_KEY_ALT:
			modifier = SUNNY_MODIFIER_LEFT_ALT;
			break;

		case SUNNY_KEY_SHIFT:
			modifier = SUNNY_MODIFIER_LEFT_SHIFT;
			break;
		}

		if (pressed)
			inputManager->m_keyModifiers |= modifier;
		else
			inputManager->m_keyModifiers &= modifier;

		if (pressed)
			inputManager->m_eventCallback(events::KeyPressedEvent(key, repeat, inputManager->m_keyModifiers));
		else
			inputManager->m_eventCallback(events::KeyReleasedEvent(key));
	}
	
	void MouseButtonCallback(InputManager* inputManager, int button, int x, int y)
	{
		bool down = false;

		switch (button)
		{
			/* DOWN */
		case WM_LBUTTONDOWN:
			SetCapture(hWnd);
			button = SUNNY_MOUSE_LEFT;
			down = true;
			break;
		case WM_MBUTTONDOWN:
			SetCapture(hWnd);
			button = SUNNY_MOUSE_MIDDLE;
			down = true;
			break;
		case WM_RBUTTONDOWN:
			SetCapture(hWnd);
			button = SUNNY_MOUSE_RIGHT;
			down = true;
			break;
			/* UP */
		case WM_LBUTTONUP:
			ReleaseCapture();
			button = SUNNY_MOUSE_LEFT;
			down = false;
			break;
		case WM_MBUTTONUP:
			ReleaseCapture();
			button = SUNNY_MOUSE_MIDDLE;
			down = false;
			break;
		case WM_RBUTTONUP:
			ReleaseCapture();
			button = SUNNY_MOUSE_RIGHT;
			down = false;
			break;
		}
	}
}