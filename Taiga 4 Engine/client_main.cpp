
#include "main.h"

void clientReceiveMain()
{
	cout << "[CLIENT_RECEIVE] Starting the client receive thread" << endl;
	sf::Packet data;
	int retVal;
	string cmd, type;
	bool parsed = false;

	while (!core.shutdown)
	{
		if (client.connected && !core.serverMode)
		{
			client.socket.setBlocking(false);
			retVal = client.socket.receive(data);
			if (retVal == sf::Socket::Done)
			{
				parsed = client.msgUnit(data);
				if (!parsed) { parsed = client.msgOrder(data); }
				if (!parsed) { parsed = client.msgGame(data); }

				data.clear();
			}
			else if (retVal == sf::Socket::Disconnected)
			{
				cout << "[CLIENT_RECEIVE] Connection to server lost!" << endl;
				client.disconnect();
			}
			else { Sleep(10); }
		}
		else { Sleep(10); }
	}

	cout << "[CLIENT_RECEIVE] Cleaning up..." << endl;
}

void clientSendMain()
{
	cout << "[CLIENT_SEND] Starting the client send thread" << endl;

	while (!core.shutdown)
	{
		if (client.connected && client.dataQueueCounter > 0)
		{
			//cout << "[CLIENT_SEND] Packet sent!" << endl;
			// Sending the data
			client.socket.send(client.dataQueue[0].data);
			// Moving the queue
			for (int i = 0; i < client.dataQueueCounter - 1; i++)
			{
				client.dataQueue[i] = client.dataQueue[i + 1];
			}
			client.dataQueueCounter -= 1;
		}
		else { Sleep(10); }
	}

	cout << "[CLIENT_SEND] Cleaning up..." << endl;
}