#include "ServerConnection.h"

namespace Chat
{
	// 1. can't accept bytes from client (problems with `select` maybe)
	int ServerConnection::retranslateReceived(bool& exitflag) //, bool& restartflag)
	{
		cout << "starting receive..." << endl;
		while (!exitflag)
		{
			std::vector<Client> current_clients;
			{
				// block only to copy clients
				std::lock_guard<std::mutex> lock(clients_mutex); // maybe using after check
				if (clients.empty())
				{
					Sleep(100);
					continue;
				}
				for (Client cl : clients)
					current_clients.push_back(cl);
			}

			// select preparation
			fd_set readfds;
			FD_ZERO(&readfds);
			for (Client s : current_clients)
				FD_SET(s.socket, &readfds);
			timeval timeout = { 1, 0 };

			int select_status = select(0, &readfds, NULL, NULL, &timeout);
			//cout << "select() returned " << select_status << endl;
			if (select_status <= 0)
				continue;

			for (Client sender : current_clients)
			{
				if (FD_ISSET(sender.socket, &readfds))
				{
					char recvbuff[DEFAULT_BUFLEN];
					int result = 0;
					result = recv(sender.socket, recvbuff, DEFAULT_BUFLEN, 0);
					if (result > 0)
					{
						string remessage = "Message from ip: ";
						remessage.append(sender.ip);
						remessage.append(":" + std::to_string(sender.port));
						remessage.append(": " + string(recvbuff, result));
						cout << remessage;
						std::lock_guard<std::mutex> lock(clients_mutex);
						for (Client& cl : clients)
						{
							int errstate = send(cl.socket, remessage.c_str(), remessage.size(), 0);
							if (errstate == SOCKET_ERROR)
							{
								cerr << "Send failed to" << cl.ip << ": " << WSAGetLastError() << endl;
								closeOnError(&cl.socket);
							}
						}
					}
					else
					{
						cerr << "Receive failed: " << WSAGetLastError()
							<< " from socket: " << sender.socket << endl;

						cout << "Client disconected: " << sender.ip << endl;
						closesocket(sender.socket);
						std::lock_guard<std::mutex> lock(clients_mutex);
						clients.erase(
							std::remove_if(
								clients.begin(),
								clients.end(),
								[sender](const Client& c)
								{
									return c.socket == sender.socket;
								}
							),
							clients.end()
						);
					}
				}
			}
		}
		return 0;
	}

	int ServerConnection::acceptClient(bool& exitflag, bool& restartflag)
	{
		Client client{};
		int errstate = 0;
		errstate = listen(Sock, SOMAXCONN);
		if (errstate != 0)
		{
			cerr << "Error in binding ip:port to socket #" << WSAGetLastError() << endl;
			return closeOnError(&(Sock));
		}

		while (!exitflag)
		{
			// select preparation
			fd_set readfds;
			FD_ZERO(&readfds);
			FD_SET(Sock, &readfds);
			timeval timeout = { 1, 0 };

			int select_status;
			select_status = select(0, &readfds, NULL, NULL, &timeout);

			if (select_status == INVALID_SOCKET)
			{
				cerr << "Select error: " << WSAGetLastError() << endl;
				continue;
			}

			if (FD_ISSET(Sock, &readfds))
			{
				int clientInfoSize = sizeof(client.info);
				ZeroMemory(&client.info, clientInfoSize);

				client.socket = accept(Sock, (sockaddr*)&client.info, &clientInfoSize); // interruption. socket from client
				if (client.socket == INVALID_SOCKET)
				{
					cerr << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << endl;
					closeOnError(&Sock);
					return closeOnError(&client.socket);
				}
				client.set_data();
				std::lock_guard<std::mutex> lock(clients_mutex);
				clients.push_back(client);
				cout << "New client connected: " << client.ip << ":" << client.port << endl;
			}
		}
		return 0;
	}

	void ServerConnection::closeServer()
	{
		cout << "Closing server: " << endl;
		for (Client cl : clients)
		{
			if (cl.socket != INVALID_SOCKET)
			{
				cout << "\tClient connected with IP address " << cl.ip << " closed" << endl;
				shutdown(cl.socket, SD_BOTH);
				closesocket(cl.socket);
			}
		}
		clients.clear();
		closesocket(Sock);
		WSACleanup();
	}

}
