#include <stdio.h>
#include <iostream>
#include <vector>
#include "Connetion.h"

namespace Chat
{

#if _WIN32
    // Only for Windows
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib ,"ws2_32.lib")


    int Connection::closeOnError(SOCKET* sock)
    {
        closesocket(*sock);
        WSACleanup();
        return 1;
    }

    int Connection::closeOnError(std::vector<SOCKET*> socks)
    {
        for (SOCKET* sock : socks)
        {
            closesocket(*sock);
        }
        WSACleanup();
        return 1;
    }

    int Connection::checkSocket()
    {
        // Check if we can use socket version
        WSAData wsadata;
        // MAKEWORD(2,2) is Winsock v2.2
        int errstate = WSAStartup(MAKEWORD(2, 2), &wsadata);

        if (errstate != 0)
        {
            std::cout << "Error WinSock version initialization #" << WSAGetLastError() << std::endl;
            return 1;
        }
        else
            return 0;
    }

    // Create a socket and put to Connection::Sock
    // uses WSAGetLastError from Winsock2 (need to change)
    int Connection::createSocket()
    {
        this->Sock = socket(AF_INET, SOCK_STREAM, 0);
        if (Sock == INVALID_SOCKET)
        {
            std::cout << "Error initialization socket #" << WSAGetLastError() << std::endl;
            return closeOnError(&Sock);
        }
        else return 0;
    }

#elif __linux__
#include <sys/socket.h>

    int Connection::closeOnError(SOCKET* sock)
    {
        closesocket(*sock);
        return 1;
    }
#endif


    // uses WSAGetLastError from Winsock2 (need to change)
    int Connection::connectSocketToAdress(char* ip, int port)
    {
        int errstate;
        // -- check if we can use IP adress
        in_addr ipChars_to_ipNum;
        errstate = inet_pton(AF_INET, ip, &ipChars_to_ipNum);
        if (errstate <= 0)
        {
            std::cout << "Error in IP convert to special numeric format. IP: " << ip << std::endl;
            return 1;
        }
        // --
        sockaddr_in servInfo;
        ZeroMemory(&servInfo, sizeof(servInfo));
        servInfo.sin_family = AF_INET;
        servInfo.sin_port = htons(port);
        servInfo.sin_addr = ipChars_to_ipNum;

        if (!IS_CLIENT)
            errstate = bind(this->Sock, (sockaddr*)&servInfo, sizeof(servInfo));
        else errstate = connect(this->Sock, (sockaddr*)&servInfo, sizeof(servInfo));

        if (errstate != 0)
        {
            std::cout << "Error in binding/connecting ip:port to socket #" << WSAGetLastError() << std::endl;
            return closeOnError(&(this->Sock));
        }
        else
            return 0;
    }


    // Return a pointer to socket
    SOCKET* Connection::getPSock()
    {
        return &(this->Sock);
    }
    // Return a copy of socket
    SOCKET Connection::getSock()
    {
        return this->Sock;
    }

    unsigned short Connection::getPort()
    {
        return this->PORT;
    }

    int Connection::closeSocket()
    {
        closesocket(this->Sock);
        std::cout << "Socket closed" << std::endl;
    }
}