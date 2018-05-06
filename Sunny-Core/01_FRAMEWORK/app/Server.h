#pragma once

#include <WinSock2.h>
#include "../include.h"
#include "../events/Event.h"

namespace sunny
{
	class Window;
	class Server;

	// std::function<리턴타입(입력 파라미터들)>
	typedef std::function<void(events::Event& event)> WindowEventCallback;

	class ServerManager
	{
	private:
		friend class Window;
		friend class Server;

	private:
		bool connected;

		SOCKET m_socket;

		char* m_ip;
		char* m_port;

		WindowEventCallback m_eventCallback;

	private:
		bool Connect();

		inline void SetIP(char* ip) { m_ip = ip; }
		inline void SetPORT(char* port) { m_port = port; }

	public:
		ServerManager();
		
		inline void SetEventCallback(const WindowEventCallback& callback) { m_eventCallback = callback; }

		

	private:
		friend void ServerPacketCallback(ServerManager* serverManager, SOCKET socket);
		friend void ServerCloseCallback (ServerManager* serverManager, SOCKET socket);

	};

	class Server
	{
	private:
		friend class ServerManager;

	private:
		static ServerManager* s_serverManager;

	public:
		inline static bool Connect(char* ip, char* port)
		{
			s_serverManager->SetIP(ip);
			s_serverManager->SetPORT(port);
			
			return s_serverManager->Connect();
		}

		inline static bool IsConnected() { return s_serverManager->connected; }

		inline static SOCKET GetSocket() { return s_serverManager->m_socket; }
	};
}
