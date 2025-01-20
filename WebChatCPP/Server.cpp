#include <stdio.h>
#include <iostream>
#include <vector>
#include "ServerConnection.h"
using namespace Chat;

BOOL CtrlHandler(DWORD event);
bool do_restart = false;
bool close_server = false;
int main()
{
    ServerConnection con = { 0 };
    if (con.checkSocket() || con.createSocket())
        return 1;

    // Connect socket to the ip:port 
    if (con.connectSocketToAdress((char*)DEFAULT_IP, DEFAULT_PORT))
        return 1;
    
    SetConsoleCtrlHandler(CtrlHandler, false);
    con.acceptClient(&close_server, &do_restart);

    WSACleanup();
    return 0;
}

BOOL CtrlHandler(DWORD event) {
    switch (event)
    {
    case CTRL_BREAK_EVENT: do_restart = true;
    case CTRL_C_EVENT:
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
    case CTRL_CLOSE_EVENT:
        close_server = true;
        
    default: return false;
    }
}