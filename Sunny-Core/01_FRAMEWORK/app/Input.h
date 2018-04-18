#pragma once

#include "../include.h"
#include "../maths/maths.h"
#include "../events/Event.h"

namespace sunny
{
#define MAX_KEYS  1024
#define MAX_BUTTONS 32

	// std::function<리턴타입(입력 파라미터들)>
	typedef std::function<void(events::Event& event)> WindowEventCallback;

	class Window;

	class InputManager
	{
	private:
		friend class Window;

	private:
		// 키보드 입력 상태
		bool m_keyState    [MAX_KEYS];
		bool m_lastKeyState[MAX_KEYS];

		// 마우스 입력 상태
		bool m_mouseButtons  [MAX_BUTTONS];
		bool m_lastMouseState[MAX_BUTTONS];
		bool m_mouseClicked  [MAX_BUTTONS];
		bool m_mouseGrabbed;
		 
		int m_keyModifiers;

		maths::vec2 m_mousePosition;

		WindowEventCallback m_eventCallback;

	public:
		InputManager();

		inline void SetEventCallback(const WindowEventCallback& callback) { m_eventCallback = callback; }
	
		void Update();

		// 입력 확인
		bool IsKeyPressed        (unsigned int keycode) const;
		bool IsMouseButtonPressed(unsigned int button ) const;
		bool IsMouseButtonClicked(unsigned int button ) const;

		// 마우스 위치
		const maths::vec2& GetMousePosition() const;
		void SetMousePosition(const maths::vec2& position);
	
		// 클리어
		void ClearKeys();
		void ClearMouseButtons();

		// 그랩
		void SetMouseGrabbed(bool grabbed);
		const bool IsMouseGrabbed() const;

		// 커서
		void SetMouseCursor(int cursor);

	private:
		friend void KeyCallback(InputManager* inputManager, int flags, int key, unsigned int message);
		friend void MouseButtonCallback(InputManager* inputManager, int button, int x, int y);
	};

	class Input
	{
	private:
		friend class InputManager;

	private:
		static InputManager* s_inputManager;

	public:
		inline static bool IsKeyPressed        (unsigned int keycode) { return s_inputManager->IsKeyPressed(keycode);        }
		inline static bool IsMouseButtonPressed(unsigned int button ) { return s_inputManager->IsMouseButtonPressed(button); }
		inline static bool IsMouseButtonClicked(unsigned int button)  { return s_inputManager->IsMouseButtonClicked(button); }

		inline static const maths::vec2& GetMousePosition() { return s_inputManager->GetMousePosition(); }

		inline static InputManager* GetInputManager() { return s_inputManager; }
	};
}

/* SUNNY_ */
#define SUNNY_MOUSE_LEFT	  0x00
#define SUNNY_MOUSE_MIDDLE	  0x01
#define SUNNY_MOUSE_RIGHT     0x02

#define SUNNY_NO_CURSOR	  NULL

#define SUNNY_MODIFIER_LEFT_CONTROL 	BIT(0)
#define SUNNY_MODIFIER_LEFT_ALT		    BIT(1)
#define SUNNY_MODIFIER_LEFT_SHIFT		BIT(2)
#define SUNNY_MODIFIER_RIGHT_CONTROL	BIT(3)
#define SUNNY_MODIFIER_RIGHT_ALT		BIT(4)
#define SUNNY_MODIFIER_RIGHT_SHIFT		BIT(5)

#define SUNNY_KEY_TAB		  0x09

#define SUNNY_KEY_0			  0x30
#define SUNNY_KEY_1			  0x31
#define SUNNY_KEY_2			  0x32
#define SUNNY_KEY_3			  0x33
#define SUNNY_KEY_4			  0x34
#define SUNNY_KEY_5			  0x35
#define SUNNY_KEY_6			  0x36
#define SUNNY_KEY_7			  0x37
#define SUNNY_KEY_8			  0x38
#define SUNNY_KEY_9			  0x39

#define SUNNY_KEY_A			  0x41
#define SUNNY_KEY_B			  0x42
#define SUNNY_KEY_C			  0x43
#define SUNNY_KEY_D			  0x44
#define SUNNY_KEY_E			  0x45
#define SUNNY_KEY_F			  0x46
#define SUNNY_KEY_G			  0x47
#define SUNNY_KEY_H			  0x48
#define SUNNY_KEY_I			  0x49
#define SUNNY_KEY_J			  0x4A
#define SUNNY_KEY_K			  0x4B
#define SUNNY_KEY_L			  0x4C
#define SUNNY_KEY_M			  0x4D
#define SUNNY_KEY_N			  0x4E
#define SUNNY_KEY_O			  0x4F
#define SUNNY_KEY_P			  0x50
#define SUNNY_KEY_Q			  0x51
#define SUNNY_KEY_R			  0x52
#define SUNNY_KEY_S			  0x53
#define SUNNY_KEY_T			  0x54
#define SUNNY_KEY_U			  0x55
#define SUNNY_KEY_V			  0x56
#define SUNNY_KEY_W			  0x57
#define SUNNY_KEY_X			  0x58
#define SUNNY_KEY_Y			  0x59
#define SUNNY_KEY_Z			  0x5A

#define SUNNY_KEY_LBUTTON        0x01
#define SUNNY_KEY_RBUTTON        0x02
#define SUNNY_KEY_CANCEL         0x03
#define SUNNY_KEY_MBUTTON        0x04

#define SUNNY_KEY_ESCAPE         0x1B
#define SUNNY_KEY_SHIFT          0x10
#define SUNNY_KEY_CONTROL        0x11
#define SUNNY_KEY_MENU           0x12
#define SUNNY_KEY_ALT	         SUNNY_KEY_MENU
#define SUNNY_KEY_PAUSE          0x13
#define SUNNY_KEY_CAPITAL        0x14

#define SUNNY_KEY_SPACE          0x20
#define SUNNY_KEY_PRIOR          0x21
#define SUNNY_KEY_NEXT           0x22
#define SUNNY_KEY_END            0x23
#define SUNNY_KEY_HOME           0x24
#define SUNNY_KEY_LEFT           0x25
#define SUNNY_KEY_UP             0x26
#define SUNNY_KEY_RIGHT          0x27
#define SUNNY_KEY_DOWN           0x28
#define SUNNY_KEY_SELECT         0x29
#define SUNNY_KEY_PRINT          0x2A
#define SUNNY_KEY_EXECUTE        0x2B
#define SUNNY_KEY_SNAPSHOT       0x2C
#define SUNNY_KEY_INSERT         0x2D
#define SUNNY_KEY_DELETE         0x2E
#define SUNNY_KEY_HELP           0x2F

#define SUNNY_KEY_NUMPAD0        0x60
#define SUNNY_KEY_NUMPAD1        0x61
#define SUNNY_KEY_NUMPAD2        0x62
#define SUNNY_KEY_NUMPAD3        0x63
#define SUNNY_KEY_NUMPAD4        0x64
#define SUNNY_KEY_NUMPAD5        0x65
#define SUNNY_KEY_NUMPAD6        0x66
#define SUNNY_KEY_NUMPAD7        0x67
#define SUNNY_KEY_NUMPAD8        0x68
#define SUNNY_KEY_NUMPAD9        0x69
#define SUNNY_KEY_MULTIPLY       0x6A
#define SUNNY_KEY_ADD            0x6B
#define SUNNY_KEY_SEPARATOR      0x6C
#define SUNNY_KEY_SUBTRACT       0x6D
#define SUNNY_KEY_DECIMAL        0x6E
#define SUNNY_KEY_DIVIDE         0x6F
#define SUNNY_KEY_F1             0x70
#define SUNNY_KEY_F2             0x71
#define SUNNY_KEY_F3             0x72
#define SUNNY_KEY_F4             0x73
#define SUNNY_KEY_F5             0x74
#define SUNNY_KEY_F6             0x75
#define SUNNY_KEY_F7             0x76
#define SUNNY_KEY_F8             0x77
#define SUNNY_KEY_F9             0x78
#define SUNNY_KEY_F10            0x79
#define SUNNY_KEY_F11            0x7A
#define SUNNY_KEY_F12            0x7B
#define SUNNY_KEY_F13            0x7C
#define SUNNY_KEY_F14            0x7D
#define SUNNY_KEY_F15            0x7E
#define SUNNY_KEY_F16            0x7F
#define SUNNY_KEY_F17            0x80
#define SUNNY_KEY_F18            0x81
#define SUNNY_KEY_F19            0x82
#define SUNNY_KEY_F20            0x83
#define SUNNY_KEY_F21            0x84
#define SUNNY_KEY_F22            0x85
#define SUNNY_KEY_F23            0x86
#define SUNNY_KEY_F24            0x87

#define SUNNY_KEY_NUMLOCK        0x90
#define SUNNY_KEY_SCROLL         0x91

#define SUNNY_KEY_LSHIFT         0xA0
#define SUNNY_KEY_RSHIFT         0xA1
#define SUNNY_KEY_LCONTROL       0xA2
#define SUNNY_KEY_RCONTROL       0xA3
#define SUNNY_KEY_LMENU          0xA4
#define SUNNY_KEY_RMENU          0xA5