
#include "define.h"

class cServerPlayer
{
public:
	int unit;
	sf::Vector2f camPos;
	sf::Vector2i camRes;

	int packet;

	int myId;
	bool connected;
	sf::TcpSocket socket;

	void disconnect();
	void addResource(float value);
	void setResource(float value);
	void setHealth(float hp);
	void setMaxHealth(float maxHp);

	void assignUnit(int globalId);

	cServerPlayer() {
		unit = -1;
		packet = 0;
		connected = false;
		socket.setBlocking(false);
	}
};

class cServer
{
public:
	cServerPlayer player[LIMIT_SERVER_PLAYERS];
	int dataQueueCounter;
	cPacketQueue dataQueue[LIMIT_SERVER_PACKETQUEUE];

	void sendPacket(int target, sf::Packet data);

	bool isLocalPlayer(int player);
	void initialize();
	void introduce(int playerId);
	void sendChunkData(int playerId, int x, int y);
	void assignUnit(int playerId, int unitId);

	// Takes: Unit global id
	// Returns: Controlling player id
	int getController(int target);
	// Takes: Player index, Data packet
	// Returns: True, if packet is parsed successfully
	bool msgRequest(int i, sf::Packet data);
	// Takes: Player index, Data packet
	// Returns: True, if packet is parsed successfully
	bool msgControlUnit(int i, sf::Packet data);
	// Takes: Player index, Data packet
	// Returns: True, if packet is parsed successfully
	bool msgControlItem(int i, sf::Packet data);
	// Takes: Player index, Data packet
	// Returns: True, if packet is parsed successfully
	bool msgControlAbility(int i, sf::Packet data);
	// Takes: Unit global id
	// Returns: Unit data ready for transfer
	sf::Packet packUnitData(int id, int variation = -1);

	cServer()
	{
		dataQueueCounter = 0;
		for (int i = 0; i < LIMIT_SERVER_PLAYERS; i++) {
			player[i].myId = i;
		}
	}
};

void serverWorldMain();
void serverConnectMain();
void serverReceiveMain();
void serverSendMain();