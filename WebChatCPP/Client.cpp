#include <thread>
#include "ClientConnection.h"
using namespace Chat;

bool close_client = false;
int main()
{
    ClientConnection con {};
    if (con.checkSocket() || con.create_connectSocket((char*)DEFAULT_IP, DEFAULT_PORT))
        return 1;

    std::thread send(&ClientConnection::sendToServer, &con, std::ref(close_client));
    std::thread recv(&ClientConnection::receiveFromServer, &con, std::ref(close_client));

    send.join();
    recv.join();

    WSACleanup();
	return 0;
}