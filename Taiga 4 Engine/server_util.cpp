
#include "main.h"

bool socketFree = true;

void cServer::sendPacket(int target, sf::Packet data)
{
	if (!core.localServer && !core.serverMode) { return; }

	while (!socketFree) { Sleep(100); }
	socketFree = false;

	dataQueue[dataQueueCounter].target = target;
	dataQueue[dataQueueCounter].data = data;
	dataQueueCounter += 1;

	socketFree = true;
}

void cServer::introduce(int playerId)
{
	// Initializing
	player[playerId].unit = -1;
	// Sending unit data
	sf::Packet data;
	// Ignore local player
	if (!server.isLocalPlayer(playerId))
	{
		for (int i = 0; i < game.unitCounter; i++)
		{
			data << MSG_UNIT_ADD << game.unit[i].globalId << game.unit[i].type << game.unit[i].pos.x << game.unit[i].pos.y << -1;
			sendPacket(playerId, data);
			data.clear();
			if (server.dataQueueCounter > 100) { Sleep(1); }
			for (int y = 0; y < game.unit[i].orderCounter; y++)
			{
				if (game.unit[i].order[y].type == ORDER_MOVETO)
				{
					data << MSG_ORDER_MOVETO << game.unit[i].globalId << game.unit[i].order[y].targetPos.x << game.unit[i].order[y].targetPos.y << true;
					sendPacket(playerId, data);
				}
				data.clear();
				if (server.dataQueueCounter > 100) { Sleep(1); }
			}
		}
	}
	// Send gamedata
	if (!server.isLocalPlayer(playerId))
	{
		data << MSG_GAME_TIME << game.timeOfDay;
		server.sendPacket(playerId, data);
		data.clear();
		data << MSG_GAME_LIGHT << game.ambientLight;
		server.sendPacket(playerId, data);
		data.clear();
	}
	// Create new unit
	string playerType = "player";
	if (core.editorMode) { playerType = "editor"; }

	server.player[playerId].unit = game.addUnit(playerType, sf::Vector2f(0, 0));
	if (!server.isLocalPlayer(playerId))
	{
		data << MSG_UNIT_HERO << server.player[playerId].unit;
		server.sendPacket(playerId, data);
		data.clear();
	}
	else {
		client.unit = server.player[playerId].unit;
	}
	server.player[playerId].setHealth(100.00f);
	server.player[playerId].setMaxHealth(100.00f);
	if (!core.editorMode)
	{
		game.unit[game.getUnitId(server.player[playerId].unit)].container.add("human_ear", 100);
		game.unit[game.getUnitId(server.player[playerId].unit)].container.add("human_arm", 50);
		game.unit[game.getUnitId(server.player[playerId].unit)].container.add("human_leg", 70);
		game.unit[game.getUnitId(server.player[playerId].unit)].container.add("weapon_rifle", 1);
		game.unit[game.getUnitId(server.player[playerId].unit)].container.add("test_resourceManipulator", 1);
	}
	else { editor.initialize(); }
	ui.updateInterface();
}

void cServerPlayer::disconnect()
{
	connected = false;
	socket.disconnect();
	// Removing the unit
	game.removeUnit(unit);
	unit = -1;
}

void cServerPlayer::addResource(float d)
{
	game.unit[game.getUnitId(unit)].addResource(d);
}

void cServerPlayer::setResource(float value)
{
	game.unit[game.getUnitId(unit)].setResource(value);
}

void cServerPlayer::setHealth(float hp)
{
	game.unit[game.getUnitId(unit)].setHealth(hp);
}

void cServerPlayer::setMaxHealth(float hp)
{
	game.unit[game.getUnitId(unit)].setMaxHealth(hp);
}

bool cServer::isLocalPlayer(int player)
{
	return (!core.serverMode && player == 0 && client.connected);
}

int cServer::getController(int target)
{
	for (int i = 0; i < LIMIT_SERVER_PLAYERS; i++)
	{
		if (player[i].connected && player[i].unit == target) { return i; }
	}
	return -1;
}