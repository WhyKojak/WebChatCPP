#pragma once
#include "Connection.h"

namespace Chat
{
	class ClientConnection : public Connection
	{
	public:
		ClientConnection(short buff_size = DEFAULT_BUFLEN) :
			Connection(true, buff_size) { }

		int sendToServer(bool& exitflag);
		int receiveFromServer(bool& exitflag);
		void closeClient();
	};
}