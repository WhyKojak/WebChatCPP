#include <stdio.h>
#include <iostream>
#include <vector>
#include "ClientConnection.h"
using namespace Chat;

int main()
{
    ClientConnection con = { 0 };
    if (con.checkSocket() || con.create_connectSocket((char*)DEFAULT_IP, DEFAULT_PORT))
        return 1;

    WSACleanup();
	return 0;
}