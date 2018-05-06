#include "Server.h"

#include "../app/Window.h"
#include "../events/Events.h"

namespace sunny
{
	extern HWND hWnd; // Window.cpp

	ServerManager* Server::s_serverManager = nullptr;

	ServerManager::ServerManager()
	: connected(false), m_ip("127.0.0.1"), m_port("7711")
	{
		Server::s_serverManager = this;
	}

	bool ServerManager::Connect()
	{
		WSAData wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);

		m_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

		SOCKADDR_IN serverAddr;
		ZeroMemory(&serverAddr, sizeof(SOCKADDR_IN));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(atoi(m_port));
		serverAddr.sin_addr.s_addr = inet_addr(m_ip);

		int result = WSAConnect(m_socket, (sockaddr*)&serverAddr, sizeof(serverAddr), NULL, NULL, NULL, NULL);
		if (result == SOCKET_ERROR) return false;;

		WSAAsyncSelect(m_socket, hWnd, WM_SOCKET, FD_CLOSE | FD_READ);

		connected = true;

		return true;
	}

	void ServerPacketCallback(ServerManager* serverManager, SOCKET socket)
	{
		serverManager->m_eventCallback(events::ServerPacketEvent(socket));
	}

	void ServerCloseCallback(ServerManager* serverManager, SOCKET socket)
	{
		serverManager->m_eventCallback(events::ServerCloseEvent(socket));
	}
}