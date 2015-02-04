
#include "define.h"

class cPacketQueue
{
public:
	int target;
	sf::Packet data;
};

class cClient
{
public:
	int dataQueueCounter;
	cPacketQueue dataQueue[LIMIT_SERVER_PACKETQUEUE];

	int unit;
	
	int packet;
	bool connected;
	sf::TcpSocket socket;

	void sendPacket(sf::Packet data);
	void connect(std::string ip, short port);
	void disconnect();

	bool msgUnit(sf::Packet data);
	bool msgOrder(sf::Packet data);
	bool msgGame(sf::Packet data);

	cClient() {
		unit = -1;
		packet = 0;
		connected = false;
		socket.setBlocking(false);

		dataQueueCounter = 0;
	}
};

void clientReceiveMain();
void clientSendMain();