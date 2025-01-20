#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>

namespace Chat
{
#if _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib ,"ws2_32.lib")

#elif __linux__
#include <sys/socket.h>
	using SOCKET = int;
#endif

#define DEFAULT_PORT 8160
#define DEFAULT_IP "127.0.0.1"
	using PORT_TYPE = unsigned short;

	class Connection
	{
	protected:
		short BUFF_SIZE;
		bool IS_CLIENT;
		SOCKET Sock;

	public:
		Connection(bool is_client, short buffer_size = 1024)
		{
			this->BUFF_SIZE = buffer_size;
			this->IS_CLIENT = is_client;
		}
		~Connection() {
			this->closeSocket();
		}
#if _WIN32
		int checkSocket();
#endif
		virtual int closeOnError(SOCKET*);
		virtual int closeOnError(std::vector<SOCKET*>);
		virtual int closeSocket();
		virtual int connectSocketToAdress(char* ip, PORT_TYPE port);
		virtual int createSocket();
		virtual int create_connectSocket(char* ip, PORT_TYPE port);
		virtual SOCKET* getPSock();
		virtual SOCKET getSock();
	};
}