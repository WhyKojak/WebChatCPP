#pragma once
#include "Connetion.h"

namespace Chat
{
	class ClientConnection : public Connection
	{
	public:
		using Connection::Connection;

		int connectToServer();
	};
}