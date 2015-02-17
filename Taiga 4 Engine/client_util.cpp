
#include "main.h"

void cClient::sendPacket(sf::Packet data)
{
	dataQueue[dataQueueCounter].data = data;
	dataQueueCounter += 1;
}

void cClient::connect(string ip, short port)
{
	if (core.serverMode) { return; }

	disconnect();

	sf::IpAddress address(ip);
	sf::Time timeout(sf::milliseconds(5000));

	cout << "[CLIENT] Connecting to " << ip << ":" << port << "..." << "\n";
	client.socket.setBlocking(true);
	if (client.socket.connect(address, port, timeout) == sf::Socket::Done)
	{
		client.connected = true;
		// Close the local server, if connected else where
		if (client.socket.getRemoteAddress().toString() != "127.0.0.1") {
			core.localServer = false;
		}
		cout << "[CLIENT] Server found!" << "\n";
	}
	else
	{
		cout << "[CLIENT] Can't reach the server!" << "\n";
	}
	client.socket.setBlocking(false);
}

void cClient::disconnect()
{
	if (core.serverMode) { return; }

	if (!core.localServer) { game.unitCounter = 0; }
	core.localServer = true;
	client.connected = false;
	client.socket.disconnect();
}