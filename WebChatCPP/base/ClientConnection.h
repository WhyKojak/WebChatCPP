#pragma once
#include "Connection.h"

namespace Chat
{
	class ClientConnection : public Connection
	{
	public:
		ClientConnection(short buff_size = 1024) : 
			Connection(true, buff_size) { }

		int connectToServer();
	};
}