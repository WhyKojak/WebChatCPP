#include "ServerConnection.h"

namespace Chat
{
    // do nothing
    int ServerConnection::listenToConnections()
    {
        return 0;
    }
    int ServerConnection::acceptClient(bool* exitflag, bool* restartflag)
	{
        while (!*exitflag)
        {
            int errstate = 0;
            errstate = listen(this->Sock, SOMAXCONN);
            if (errstate != 0)
            {
                std::cout << "Error in binding ip:port to socket #" << WSAGetLastError() << std::endl;
                return closeOnError(&(this->Sock));
            }
            sockaddr_in clientInfo;
            int clientInfoSize = sizeof(clientInfo);
            ZeroMemory(&clientInfo, clientInfoSize);

            SOCKET clientConn = accept(this->Sock, (sockaddr*)&clientInfo, &clientInfoSize); // interuption. socket from client

            if (clientConn == INVALID_SOCKET)
            {
                std::cout << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << std::endl;
                closeOnError(&Sock);
                //
                return closeOnError(&clientConn);
            }
            else
            {
                this->clientSockets.push_back(clientConn);
                std::cout << "Connection to a client established successfully" << std::endl;
                char clientIP[22];
                inet_ntop(AF_INET, &clientInfo.sin_addr, clientIP, INET_ADDRSTRLEN);	// Convert connected client's IP to standard string format
                std::cout << "Client connected with IP address " << clientIP << std::endl;
                //
                return 0;
            }
        }
	}
}