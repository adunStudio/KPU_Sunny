#include "ServerEvent.h"

namespace sunny
{
	namespace events
	{
		ServerEvent::ServerEvent(SOCKET socket, Event::Type type)
		: Event(type), socket(socket)
		{
		}

		ServerPacketEvent::ServerPacketEvent(SOCKET socket)
		: ServerEvent(socket, ServerPacketEvent::GetStaticType())
		{

		}

		ServerCloseEvent::ServerCloseEvent(SOCKET socket)
		: ServerEvent(socket, ServerCloseEvent::GetStaticType())
		{

		}

	}
}