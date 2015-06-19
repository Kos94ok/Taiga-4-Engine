
#include "main.h"

bool cClient::msgBig(sf::Packet input)
{
	int msg, size, id;
	int argi[] = { 0, 0, 0, 0 };
	float argf[] = { 0.00f, 0.00f, 0.00f, 0.00f };
	string type;

	input >> msg;
	// ============================================
	// ============================================
	// World chunk data packet
	if (msg == MSG_BIG_WORLD_ALLCHUNKS)
	{
		input >> size;
		for (int y = 0; y < size; y++)
		{
			for (int x = 0; x < size; x++)
			{
				input >> argi[0];
				world.map[x][y].type = argi[0];
			}
		}
	}
	// ============================================
	// ============================================
	// Units in chunk
	if (msg == MSG_BIG_WORLD_ONECHUNK)
	{
		game.access.lock();
		int count = 0, dup = 0;
		while (!input.endOfPacket())
		{
			input >> id >> type >> argf[0] >> argf[1] >> argi[2] >> argi[3];
			if (game.getUnitId(id) == -1)
			{
				game.addUnit(type, sf::Vector2f(argf[0], argf[1]), argi[2], argi[3], false, id);
				count += 1;
			}
			else { dup += 1; }
		}
		game.access.unlock();
		//console.debug << "[DEBUG] Units added: " << count << ", duplicated: " << dup << endl;
	}

	return false;
}

bool cClient::msgUnit(sf::Packet input)
{
	int argi[] = { 0, 0, 0, 0 };
	float argf[] = { 0.00f, 0.00f, 0.00f, 0.00f };
	bool argb[] = { false, false, false, false };
	int msg, id;
	string cmd, type;

	input >> msg;
	// ============================================
	// ============================================
	// Create unit
	if (msg == MSG_UNIT_ADD)
	{
		input >> id >> type >> argf[0] >> argf[1] >> argi[2] >> argi[3];
		if (game.getUnitId(id) == -1)
		{
			game.addUnit(type, sf::Vector2f(argf[0], argf[1]), argi[2], argi[3], false, id);
		}
		else { console.error << "[cClient::msgUnit / MSG_UNIT_ADD] Duplicate create unit message. Ignoring" << "\n"; }
		return true;
	}
	// ============================================
	// ============================================
	// Remove unit
	if (msg == MSG_UNIT_REMOVE)
	{
		input >> id;
		game.removeUnit(id, false);
		return true;
	}
	// ============================================
	// ============================================
	// Get our hero
	if (msg == MSG_UNIT_HERO)
	{
		input >> id;
		client.unit = id;
		return true;
	}
	// ============================================
	// ============================================
	// Get unit resource
	if (msg == MSG_UNIT_SETRESOURCE)
	{
		input >> id >> argf[0];
		game.unit[game.getUnitId(id)].setResource(argf[0]);
		console << argf[0] << "\n";
		return true;
	}
	// ============================================
	// ============================================
	// Get unit health
	if (msg == MSG_UNIT_SETHEALTH)
	{
		input >> id >> argf[0];
		id = game.getUnitId(id);
		if (id != -1) {
			game.unit[id].setHealth(argf[0]);
		}
		return true;
	}
	// ============================================
	// ============================================
	// Get max unit health
	if (msg == MSG_UNIT_SETMAXHEALTH)
	{
		input >> id >> argf[0];
		id = game.getUnitId(id);
		if (id != -1) {
			game.unit[id].setMaxHealth(argf[0]);
		}
		return true;
	}
	// ============================================
	// ============================================
	// Add item to unit
	if (msg == MSG_UNIT_ADDITEM)
	{
		input >> id >> type >> argi[0];
		int changeId = game.getUnitId(id);
		if (changeId != -1) {
			game.unit[changeId].container.add(type, argi[0]);
		}
		//else { console.error << "[cClient::msgUnit / MSG_UNIT_ADDITEM] Can't find target unit (" << id << ")!\n"; }
		return true;
	}
	// ============================================
	// ============================================
	// Remove item from unit
	if (msg == MSG_UNIT_REMOVEITEM)
	{
		input >> id >> type >> argi[0];
		id = game.getUnitId(id);
		if (id != -1) {
			game.unit[id].container.remove(type, argi[0]);
		}
		return true;
	}
	// ============================================
	// ============================================
	// Move unit to new position
	if (msg == MSG_UNIT_MOVETO)
	{
		input >> id >> argf[0] >> argf[1];
		id = game.getUnitId(id);
		if (id != -1) {
			game.unit[id].moveTo(vec2(argf[0], argf[1]));
		}
		return true;
	}
	return false;
}

bool cClient::msgOrder(sf::Packet input)
{
	int argi[] = { 0, 0, 0, 0 };
	float argf[] = { 0.00f, 0.00f, 0.00f, 0.00f };
	bool argb[] = { false, false, false, false };
	int msg, id;
	string cmd, type;

	input >> msg;
	// ============================================
	// ============================================
	// Order unit to move
	if (msg == MSG_ORDER_MOVETO)
	{
		input >> id >> argf[0] >> argf[1] >> argb[2];
		id = game.getUnitId(id);
		if (id != -1)
		{
			game.unit[id].addOrder_moveto(sf::Vector2f(argf[0], argf[1]), argb[2]);
		}
		return true;
	}
	// ============================================
	// ============================================
	// Order unit to move with pathing
	if (msg == MSG_ORDER_MOVETOPATH)
	{
		input >> id >> argf[0] >> argf[1] >> argb[2];
		id = game.getUnitId(id);
		if (id != -1)
		{
			game.unit[id].addOrder_moveto_path(sf::Vector2f(argf[0], argf[1]), argb[2]);
		}
		return true;
	}
	// ============================================
	// ============================================
	// Order unit to pick up the item
	if (msg == MSG_ORDER_PICKUP)
	{
		input >> id >> argi[0] >> argb[1];
		id = game.getUnitId(id);
		if (id != -1)
		{
			game.unit[id].addOrder_pickup(argi[0], argb[1]);
		}
		return true;
	}
	// ============================================
	// ============================================
	// Order unit to harvest resource from object
	if (msg == MSG_ORDER_HARVEST)
	{
		input >> id >> argi[0] >> argb[1] >> argb[2];
		id = game.getUnitId(id);
		if (id != -1)
		{
			game.unit[id].addOrder_harvest(argi[0], argb[1], argb[2]);
		}
		return true;
	}
	// ============================================
	// ============================================
	// Order unit to stop
	if (msg == MSG_ORDER_STOP)
	{
		input >> id;
		id = game.getUnitId(id);
		if (id != -1)
		{
			game.unit[id].removeAllOrders();
		}
		return true;
	}
	// ============================================
	// ============================================
	// Order unit to die
	if (msg == MSG_ORDER_DEATH)
	{
		input >> id >> argb[0];
		id = game.getUnitId(id);
		if (id != -1)
		{
			game.unit[id].addOrder_death(argb[0]);
		}
		return true;
	}
	return false;
}

bool cClient::msgGame(sf::Packet input)
{
	int argi[] = { 0, 0, 0, 0 };
	float argf[] = { 0.00f, 0.00f, 0.00f, 0.00f };
	bool argb[] = { false, false, false, false };
	int msg;
	string cmd, type, argStr;

	input >> msg;
	// ============================================
	// ============================================
	// Echo
	if (msg == MSG_ECHO)
	{
		input >> argi[0] >> argStr;
		if (argi[0] == -1) {
			console.echo << "[ECHO] Server: " << argStr << endl;
		}
		else {
			console.echo << "[ECHO] Player " << argi[0] << ": " << argStr << endl;
		}

		return true;
	}
	// ============================================
	// ============================================
	// Ping
	if (msg == MSG_PING)
	{
		input.clear();
		input << MSG_PONG;
		sendPacket(input);
		input.clear();

		return true;
	}
	// ============================================
	// ============================================
	// Pong
	if (msg == MSG_PONG)
	{
		client.lastPongTime = timeGetTime();
		client.ping = client.lastPongTime - client.lastPingTime;

		return true;
	}
	// ============================================
	// ============================================
	// Console command
	if (msg == MSG_CMD)
	{
		input >> cmd;
		console.parseCommand(cmd);
		return true;
	}
	// ============================================
	// ============================================
	// Force update interface
	if (msg == MSG_UI_UPDATEITEMLIST)
	{
		ui.updateInterfaceItemList();
		ui.updateInterfaceEquipment();
		return true;
	}
	// ============================================
	// ============================================
	// Game time
	if (msg == MSG_GAME_TIME)
	{
		input >> argf[0];
		game.timeOfDay = argf[0];
		return true;
	}
	// ============================================
	// ============================================
	// Game light
	if (msg == MSG_GAME_LIGHT)
	{
		input >> argf[0];
		game.ambientLight = argf[0];
		return true;
	}
	// ============================================
	// ============================================
	// Move camera
	if (msg == MSG_CAMERA_MOVETO)
	{
		input >> argf[0] >> argf[1];
		camera.moveto(vec2f(argf[0], argf[1]));
		return true;
	}
	return false;
}