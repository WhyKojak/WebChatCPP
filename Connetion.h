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

#endif

#define DEFAULT_PORT 7090

	class Connection
	{
	protected:
		short BUFF_SIZE;
		unsigned short PORT;
		bool IS_CLIENT = null;
		SOCKET Sock;

	public:
		Connection(short buffer_size = 1024, unsigned short port = DEFAULT_PORT) {
			Connection::BUFF_SIZE = buffer_size;
			Connection::PORT = port;
		}
#if _WIN32
		virtual int closeOnError(SOCKET*);
		virtual int checkSocket();
#endif
		virtual int connectSocketToAdress(char* ip, int port);
		virtual int createSocket();
		virtual SOCKET* getPSock();
		virtual SOCKET getSock();
		virtual unsigned short getPort();
	};
}