#include <stdio.h>
#include <iostream>
#include <vector>
#include "Connetion.h"

using namespace Chat;

int main()
{
    
    Connection con = {0};
    if (con.checkSocket() || con.createSocket())
        return 1;

    // Connect socket to the ip:port 
    if (con.connectSocketToAdress((char*)"127.0.0.1", 57554)) 
        return 1;

    
    closesocket(Sock);
    closesocket(clientConn);
    WSACleanup();
    return 0;
}