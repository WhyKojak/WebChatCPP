#pragma once
#include "Connection.h"

namespace Chat
{
	class ServerConnection : public Connection
	{
		std::vector<SOCKET> clientSockets;
	public:
		ServerConnection(short buff_size=1024) : 
			Connection(false, buff_size) { }

		// loop listen function
		int listenToConnections();
		int acceptClient(bool* exitflag, bool* restartflag);
	};

}