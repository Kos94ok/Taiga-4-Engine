
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

	/*
	Connecting to 'localserver' means that we are connecting to the same instance
	of the game. At the same time, '127.0.0.1' or 'localhost' will mean other
	instance running on the same machine.
	*/
	bool localServer = false;
	if (ip == "localserver") { localServer = true; ip = "127.0.0.1"; }

	sf::IpAddress address(ip);
	sf::Time timeout(sf::milliseconds(5000));

	console << "[cClient::connect] Connecting to " << ip << ":" << port << "..." << "\n";
	client.socket.setBlocking(true);
	if (client.socket.connect(address, port, timeout) == sf::Socket::Done)
	{
		client.connected = true;
		core.localServer = localServer;
		console << "[cClient::connect] Server found!" << "\n";
	}
	else
	{
		console << "[cClient::connect] Can't reach the server!" << "\n";
	}
	client.socket.setBlocking(false);

	// Sending client info
	sf::Packet data;
	data << MSG_INFO_CAMRES << camera.res.x << camera.res.y;
	client.sendPacket(data);
	data.clear();
}

void cClient::disconnect()
{
	if (core.serverMode) { return; }

	if (!core.localServer) { game.unitCounter = 0; }
	core.localServer = true;
	client.connected = false;
	client.socket.disconnect();
}