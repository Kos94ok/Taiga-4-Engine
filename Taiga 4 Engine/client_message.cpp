
#include "main.h"

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
			game.addUnit(type, sf::Vector2f(argf[0], argf[1]), argi[2], argi[3]);
			game.unit[game.unitCounter - 1].globalId = id;
			game.unit[game.unitCounter - 1].owner = argi[2];
		}
		else { cout << "[CLIENT_RECEIVE] Duplicate create unit message. Ignoring" << "\n"; }
		return true;
	}
	// ============================================
	// ============================================
	// Remove unit
	if (msg == MSG_UNIT_REMOVE)
	{
		input >> id;
		game.removeUnit(id);
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
		cout << argf[0] << "\n";
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
		id = game.getUnitId(id);
		if (id != -1) {
			game.unit[id].container.add(type, argi[0]);
		}
		else { cout << "[cClient::msgUnit / MSG_UNIT_ADDITEM] Can't find target unit!\n"; }
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
		input >> id >> argi[0] >> argb[1];
		id = game.getUnitId(id);
		if (id != -1)
		{
			game.unit[id].addOrder_harvest(argi[0], argb[1]);
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
	return false;
}

bool cClient::msgGame(sf::Packet input)
{
	int argi[] = { 0, 0, 0, 0 };
	float argf[] = { 0.00f, 0.00f, 0.00f, 0.00f };
	bool argb[] = { false, false, false, false };
	int msg;
	string cmd, type;

	input >> msg;
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
	return false;
}