#pragma once
#include "Connection.h"
namespace Chat
{
	class ServerConnection : public Connection
	{
		struct Client
		{
			SOCKET socket;
			sockaddr_in info;
			char ip[22];
			PORT_TYPE port;
			void set_data ()
			{
				// Convert connected client's IP to standard string format
				inet_ntop(AF_INET, &info.sin_addr, (char*)ip, INET_ADDRSTRLEN);
				port = info.sin_port;
			}
		};

		std::mutex clients_mutex;
		std::vector<Client> clients;

	public:
		ServerConnection(short buff_size=DEFAULT_BUFLEN) :
			Connection(false, buff_size) { }

		// loop listen function
		int retranslateReceived(bool& exitflag); // bool& restartflag);
		int acceptClient(bool& exitflag, bool& restartflag);
		void closeServer();
	};

}