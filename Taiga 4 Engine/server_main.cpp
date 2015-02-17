
#include "main.h"

// Server thread that waits for incoming connections
void serverConnectMain()
{
	cout << "[SRV_CONNECT] Starting the server connect thread" << "\n";

	sf::TcpListener listener;
	listener.setBlocking(false);
	if (listener.listen(21045) == sf::Socket::Done)
	{
		cout << "[SRV_CONNECT] Listening at port " << listener.getLocalPort() << ".\n";
	}
	while (!core.shutdown)
	{
		for (int i = 0; i < LIMIT_SERVER_PLAYERS; i++)
		{
			if (!server.player[i].connected)
			{
				if (listener.accept(server.player[i].socket) == sf::Socket::Done)
				{
					server.player[i].packet = 0;
					server.player[i].connected = true;
					cout << "[SRV_CONNECT] Connection from IP [" << server.player[i].socket.getRemoteAddress().toString()
						<< "] assigned to slot " << i << "." << "\n";
					// Introducing the player
					server.introduce(i);
				}
				i = LIMIT_SERVER_PLAYERS;
			}
		}
		Sleep(10);
	}
	cout << "[SRV_CONNECT] Cleaning up" << "\n";
	listener.close();
	for (int i = 0; i < LIMIT_SERVER_PLAYERS; i++)
	{
		server.player[i].socket.disconnect();
	}
}

// Server thread that waits for incoming messages from clients
void serverReceiveMain()
{
	cout << "[SRV_RECEIVE] Starting the server receive thread" << "\n";
	sf::Packet data;
	int retVal = 0;
	bool parsed = false;

	while (!core.shutdown)
	{
		// Looking for data to be received
		for (int i = 0; i < LIMIT_SERVER_PLAYERS; i++)
		{
			if (server.player[i].connected)
			{
				retVal = server.player[i].socket.receive(data);
				if (retVal == sf::Socket::Done)
				{
					parsed = server.msgControlUnit(i, data);
					if (!parsed) { parsed = server.msgControlItem(i, data); }
					if (!parsed) { parsed = server.msgControlAbility(i, data); }

					data.clear();
				}
				else if (retVal == sf::Socket::Disconnected)
				{
					cout << "[SRV_RECEIVE] Player " << i << " disconnected!" << "\n";
					server.player[i].disconnect();
				}
			}
		}
	}

	cout << "[SRV_RECEIVE] Cleaning up" << "\n";
}

// Server thread that sends messages to client from data queue
void serverSendMain()
{
	sf::Packet localData;
	cout << "[SRV_SEND] Starting the server send thread" << "\n";
	while (!core.shutdown)
	{
		if (server.dataQueueCounter > 0)
		{
			// Send to everyone
			if (server.dataQueue[0].target == PLAYERS_ALL || server.dataQueue[0].target == PLAYERS_REMOTE)
			{
				int firstIndex = 0;
				if (server.dataQueue[0].target == PLAYERS_REMOTE && core.localServer && client.connected) { firstIndex = 1; }
				for (int i = firstIndex; i < LIMIT_SERVER_PLAYERS; i++)
				{
					if (server.player[i].connected)
					{
						//localData << server.player[i].packet++;
						localData.append(server.dataQueue[0].data.getData(), server.dataQueue[0].data.getDataSize());
						server.player[i].socket.send(localData);
						localData.clear();
					}
				}
			}
			// Send to specific client
			else
			{
				if (server.player[server.dataQueue[0].target].connected)
				{
					//localData << server.player[server.dataQueue[0].target].packet++;
					localData.append(server.dataQueue[0].data.getData(), server.dataQueue[0].data.getDataSize());
					server.player[server.dataQueue[0].target].socket.send(localData);
					localData.clear();
				}
			}
			// Moving the packets in queue
			for (int i = 0; i < server.dataQueueCounter - 1; i++)
			{
				server.dataQueue[i] = server.dataQueue[i + 1];
			}
			server.dataQueueCounter -= 1;
		}
		else { Sleep(10); }
	}

	cout << "[SRV_SEND] Cleaning up" << "\n";
}