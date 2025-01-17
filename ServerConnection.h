#pragma once
#include "Connetion.h"

namespace Chat
{
	class ServerConnection : public Connection
	{

	public:
		ServerConnection(short buff_size=1024, unsigned short port=7090) : 
			Connection(buff_size, port) { }

		int acceptClient();
	};

}