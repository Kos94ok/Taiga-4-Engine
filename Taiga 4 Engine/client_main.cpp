
#include "main.h"

void clientReceiveMain()
{
	int threadId = 5;
	cout << "[CLIENT_RECEIVE] Starting the client receive thread" << "\n";
	sf::Packet data;
	int retVal;
	string cmd, type;
	bool parsed = false;
	int counter = 0;

	while (!core.shutdown)
	{
		if (client.connected && !core.serverMode)
		{
			client.socket.setBlocking(false);
			retVal = client.socket.receive(data);
			if (retVal == sf::Socket::Done)
			{
				//cout << "Received to client " << counter++ << endl;
				parsed = client.msgUnit(data);
				if (!parsed) { parsed = client.msgOrder(data); }
				if (!parsed) { parsed = client.msgGame(data); }

				data.clear();
			}
			else if (retVal == sf::Socket::Disconnected)
			{
				cout << "[CLIENT_RECEIVE] Connection to server lost!" << "\n";
				client.disconnect();
			}
			else { Sleep(10); }
		}
		else { Sleep(10); }
		core.thread_antifreeze[threadId] = 0;
	}

	cout << "[CLIENT_RECEIVE] Cleaning up..." << "\n";
}

void clientSendMain()
{
	int threadId = 6;
	cout << "[CLIENT_SEND] Starting the client send thread" << "\n";

	while (!core.shutdown)
	{
		if (client.connected && client.dataQueueCounter > 0)
		{
			//cout << "[CLIENT_SEND] Packet sent!" << "\n";
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
		core.thread_antifreeze[threadId] = 0;
	}

	cout << "[CLIENT_SEND] Cleaning up..." << "\n";
}