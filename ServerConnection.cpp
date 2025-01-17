#include "ServerConnection.h"

namespace Chat
{
	int ServerConnection::acceptClient()
	{
        int errstate = 0;
        // Listeting to connections
        errstate = listen(this->Sock, SOMAXCONN); // code don't interrupt
        if (errstate != 0)
        {
            std::cout << "Error in binding ip:port to socket #" << WSAGetLastError() << std::endl;
            return closeOnError(&(this->Sock));
        }

        sockaddr_in clientInfo;
        int clientInfoSize = sizeof(clientInfo);

        SOCKET clientConn = accept(this->Sock, (sockaddr*)&clientInfo, &clientInfoSize); // interuption. socket from client

        if (clientConn == INVALID_SOCKET)
        {
            std::cout << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << std::endl;
            closeOnError(&Sock);
            closeOnError(&clientConn);
        }
        else
        {
            std::cout << "Connection to a client established successfully" << std::endl;
            char clientIP[22];
            inet_ntop(AF_INET, &clientInfo.sin_addr, clientIP, INET_ADDRSTRLEN);	// Convert connected client's IP to standard string format
            std::cout << "Client connected with IP address " << clientIP << std::endl;
        }

        //Exchange text data between Server and Client. Disconnection if a client send "xxx"

        //std::vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);							// Creation of buffers for sending and receiving data
        //short packet_size = 0;												// The size of sending / receiving packet in bytes

        //while (true) {
        //    packet_size = recv(clientConn, servBuff.data(), servBuff.size(), 0);					// Receiving packet from client. Program is waiting (system pause) until receive
        //    std::cout << "Client's message: " << servBuff.data() << std::endl;

        //    std::cout << "Your (host) message: ";
        //    fgets(clientBuff.data(), clientBuff.size(), stdin);

        //    // Check whether server would like to stop chatting 
        //    if (clientBuff[0] == 'x' && clientBuff[1] == 'x' && clientBuff[2] == 'x') {
        //        shutdown(clientConn, SD_BOTH);
        //        closesocket(Sock);
        //        closesocket(clientConn);
        //        WSACleanup();
        //        return 0;
        //    }

        //    packet_size = send(clientConn, clientBuff.data(), clientBuff.size(), 0);

        //    if (packet_size == SOCKET_ERROR) {
        //        std::cout << "Can't send message to Client. Error # " << WSAGetLastError() <<std::endl;
        //        closesocket(Sock);
        //        closesocket(clientConn);
        //        WSACleanup();
        //        return 1;
        //    }

    }

    closesocket(Sock);
    closesocket(clientConn);
    WSACleanup();
	}
}