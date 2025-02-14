#include <iostream>
#include <thread>
#include "ServerConnection.h"
using namespace Chat;
using std::cout;
using std::endl;
using std::ref;
using std::thread;

BOOL CtrlHandler(DWORD event);
bool do_restart = false;
bool close_server = false;

int main()
{
    ServerConnection con {};
    if (con.checkSocket() || con.create_connectSocket((char*)DEFAULT_IP, DEFAULT_PORT))
        return 1;
    if (!SetConsoleCtrlHandler(CtrlHandler, true))
    {
        cout << "Error: can't set control handler" << endl;
        return 1;
    }
    thread accept(&ServerConnection::acceptClient, &con, ref(close_server), ref(do_restart));
    thread retranslate(&ServerConnection::retranslateReceived, &con, ref(close_server));

    accept.join();
    retranslate.join();

    con.closeServer();

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
        cout<<"close:"<<endl;
        close_server = true;
        break;
    default:
        cout << "default ctrl handle" << endl;
        return false;
    }
    return true;
}