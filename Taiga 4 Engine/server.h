
#pragma once
#include "precomp.h"
#include "game.h"
#include "util.h"

class cServerPlayer
{
public:
	int unit;
	std::string name;
	sf::Vector2f camPos;
	sf::Vector2i camRes;
	sf::Vector2f mousePos;

	int packet;

	int myId;
	bool connected;
	sf::TcpSocket socket;
	int ping;
	int lastPingTime;
	int lastPongTime;

	void disconnect();
	void addResource(float value);
	void setResource(float value);
	void setHealth(float hp);
	void setMaxHealth(float maxHp);
	void addItem(std::string type, int count = 1);
	bool hasItem(std::string type, int count = 1);
	void moveCamera(sf::Vector2f target);
	void addBuff(int type, float duration = -1.00f, int power = 1);
	void removeBuff(int type);

	cServerPlayer() {
		unit = -1;
		ping = -1;
		packet = 0;
		connected = false;
		socket.setBlocking(false);
		name = "Unknown Player";
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
	void sendWorldData(int playerId);
	void assignUnit(int playerId, int unitId);

	void sendEcho(int source, std::string str);

	int pingTimer;
	void pingPlayers();

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
	// Takes: Unit global id, Unit variation
	// Returns: Nothing
	void addUnitData(sf::Packet* data, cUnit* unit, int variation = -1);
	// Takes: Unit global id, Unit variation
	// Returns: Unit data ready for transfer
	sf::Packet packUnitData(int id, int variation = -1);

	cServer()
	{
		pingTimer = 0;
		dataQueueCounter = 0;
		for (int i = 0; i < LIMIT_SERVER_PLAYERS; i++) {
			player[i].myId = i;
		}
	}
};

extern cServer server;

void serverWorldMain();
void serverConnectMain();
void serverReceiveMain();
void serverSendMain();