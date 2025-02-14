#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

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

using std::cout;
using std::cerr;
using std::endl;
using std::string;

const short DEFAULT_PORT = 8160;
const char DEFAULT_IP[] = "127.0.0.1";
const short DEFAULT_BUFLEN = 1024;
const short TIMEOUT = 2;
using PORT_TYPE = u_short;


	class Connection
	{
	protected:
		short BUFF_SIZE;
		bool IS_CLIENT;
		SOCKET Sock;

	public:
		Connection(bool is_client, short buffer_size = DEFAULT_BUFLEN)
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
		virtual int createSocket();
		virtual int connectSocketToAdress(char* ip, PORT_TYPE port);
		// uses creteSocket and connectSocketToAdress
		virtual int create_connectSocket(char* ip, PORT_TYPE port);
		virtual SOCKET* getPSock();
		virtual SOCKET getSock();
	};
}