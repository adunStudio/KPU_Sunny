#pragma once

#include "Event.h"

#include <WinSock2.h>

namespace sunny
{
	namespace events
	{
		class ServerEvent : public Event
		{
		public: 
			SOCKET socket;

			ServerEvent(SOCKET socket, Event::Type type);

			inline static int GetStaticType() { return static_cast<int>(Event::Type::SUNNY_SERVER_PACKET) | static_cast<int>(Event::Type::SUNNY_SERVER_CLOSE); }
		};

		class ServerPacketEvent : public ServerEvent
		{
		public:
			ServerPacketEvent(SOCKET socket);

			inline static Event::Type GetStaticType() { return Event::Type::SUNNY_SERVER_PACKET; }
		};

		class ServerCloseEvent : public ServerEvent
		{
		public:
			ServerCloseEvent(SOCKET socket);

			inline static Event::Type GetStaticType() { return Event::Type::SUNNY_SERVER_CLOSE; }
		};
	}
}