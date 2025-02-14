#include "ClientConnection.h"

namespace Chat
{
	int ClientConnection::sendToServer(bool& exitflag)
	{
		string sendmsg;
		int errstate = 0;
		while (!exitflag)
		{
			cout << ": ";
			std::getline(std::cin, sendmsg);
			errstate = send(this->Sock, sendmsg.c_str(), sendmsg.size(), 0);
			if (errstate == SOCKET_ERROR)
			{
				cerr << "Send failed: " << WSAGetLastError() << endl;
				return closeOnError(this->getPSock());
			}
			cout << "Bytes send: " << errstate << endl;
		}
	}

	int ClientConnection::receiveFromServer(bool& exitflag)
	{
		char recvbuff[DEFAULT_BUFLEN];
		int result = 0;
		while (!exitflag)
		{
			result = recv(this->Sock, recvbuff, DEFAULT_BUFLEN, 0);
			if (result > 0)
				cout << string(recvbuff, result) << endl;
			else
			{
				if (WSAGetLastError() == 0)
				{
					closeClient();
					exitflag = true;
					return 0;
				}
				else
					cerr << "Receive failed: " << WSAGetLastError() << endl;
			}
		} 
		return 0;
	}

	void ClientConnection::closeClient()
	{
		int errstate = shutdown(this->Sock, SD_BOTH);
		if (errstate == SOCKET_ERROR)
		{
			cerr << "shotdown failed: " << WSAGetLastError() << endl;
		}
		cout<<"leaving chat...";
		closesocket(this->Sock);
		WSACleanup();
	}
}

