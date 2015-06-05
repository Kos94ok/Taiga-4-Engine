
#include "main.h"

bool cServer::msgRequest(int i, sf::Packet input)
{
	int argi[] = { 0, 0, 0, 0 };
	//float argf[] = { 0.00f, 0.00f, 0.00f, 0.00f };
	//bool argb[] = { false, false, false, false };
	int msg, id;
	string cmd = "";
	sf::Packet data;

	input >> msg;
	// =======================================================
	// =======================================================
	// Request for chunk data
	if (msg == MSG_REQUEST_CHUNKDATA)
	{
		input >> argi[0] >> argi[1];
		sendChunkData(i, argi[0], argi[1]);

		return true;
	}
	// =======================================================
	// =======================================================
	// Client screen resolution
	if (msg == MSG_INFO_CAMRES)
	{
		input >> argi[0] >> argi[1];
		player[i].camRes = vec2i(argi[0], argi[1]);

		return true;
	}
	return false;
}

bool cServer::msgControlUnit(int i, sf::Packet input)
{
	int argi[] = { 0, 0, 0, 0 };
	float argf[] = { 0.00f, 0.00f, 0.00f, 0.00f };
	bool argb[] = { false, false, false, false };
	int msg, id;
	string cmd = "";
	sf::Packet data;

	input >> msg;
	// =======================================================
	// =======================================================
	// Player controls his character
	if (msg == MSG_CONTROLS_MOVETO)
	{
		input >> argf[0] >> argf[1] >> argb[2];
		id = game.getUnitId(server.player[i].unit);
		if (id != -1)
		{
			game.access.lock();
			game.unit[id].addOrder_moveto_path(sf::Vector2f(argf[0], argf[1]), !argb[2]);
			game.access.unlock();
		}
		return true;
	}
	// =======================================================
	// =======================================================
	// Player wants to stop
	if (msg == MSG_CONTROLS_STOP)
	{
		id = game.getUnitId(server.player[i].unit);
		if (id != -1)
		{
			game.unit[id].removeAllOrders();
		}
		return true;
	}
	// =======================================================
	// =======================================================
	// Player wants to pick up the item
	if (msg == MSG_CONTROLS_PICKUP)
	{
		input >> argi[0] >> argb[1];
		id = game.getUnitId(server.player[i].unit);
		int itemId = game.getUnitId(argi[0]);
		if (id != -1 && itemId != -1) {
			cUnit* playerUnit = &game.unit[id];
			cUnit* targetItem = &game.unit[itemId];
			// Check distance
			if (math.getDistance(playerUnit, targetItem) <= game.getUnitInteractDistance(*playerUnit, *targetItem))
			{
				// Units are close enough
				game.unit[id].addOrder_pickup(argi[0], !argb[1]);
			}
			else
			{
				// Unit needs to move closer
				game.unit[id].addOrder_moveto(game.getUnitInteractPoint(*playerUnit, *targetItem), !argb[1]);
				game.unit[id].addOrder_pickup(argi[0], false);
			}
		}
		else { cout << "[cServer::msgControlUnit / MSG_CONTROLS_PICKUP] Incorrect unit IDs!" << endl; }
		return true;
	}
	// =======================================================
	// =======================================================
	// Player wants to harvest some resources
	if (msg == MSG_CONTROLS_HARVEST)
	{
		input >> argi[0] >> argb[1];
		id = game.getUnitId(server.player[i].unit);
		int targetId = game.getUnitId(argi[0]);
		if (id != -1 && targetId != -1) {
			cUnit* playerUnit = &game.unit[id];
			cUnit* targetUnit = &game.unit[targetId];
			// Check distance
			if (math.getDistance(playerUnit, targetUnit) <= game.getUnitInteractDistance(*playerUnit, *targetUnit))
			{
				// Units are close enough
				game.unit[id].addOrder_harvest(argi[0], !argb[1]);
			}
			else
			{
				// Unit needs to move closer
				game.unit[id].addOrder_moveto_path(game.getUnitInteractPoint(*playerUnit, *targetUnit), !argb[1]);
				game.unit[id].addOrder_harvest(argi[0], false);
			}
		}
		else { cout << "[cServer::msgControlUnit / MSG_CONTROLS_HARVEST] Incorrect unit IDs!" << endl; }
		return true;
	}
	// =======================================================
	// =======================================================
	// Player moves the camera
	if (msg == MSG_CONTROLS_CAMERA)
	{
		input >> argf[0] >> argf[1];
		server.player[i].camPos = vec2f(argf[0], argf[1]);
	}
	return false;
}

bool cServer::msgControlItem(int i, sf::Packet input)
{
	int argi[] = { 0, 0, 0, 0 };
	float argf[] = { 0.00f, 0.00f, 0.00f, 0.00f };
	bool argb[] = { false, false, false, false };
	int msg, id, unitId;
	string cmd = "", type = "";
	sf::Packet data;
	cItemConsume consumeData;
	cItemDismantle dismantleData;

	input >> msg;
	// =======================================================
	// =======================================================
	// Player tries to craft an item
	if (msg == MSG_CONTROLS_CRAFTITEM)
	{
		input >> argi[0] >> argi[1];
		// Changing the items
			// Add crafted item
		game.getUnit(server.player[i].unit).addItem(craft.recipe[argi[0]].result.type, craft.recipe[argi[0]].result.count * argi[1]);

			// Remove ingridients
		for (int y = 0; y < craft.recipe[argi[0]].ingrCount; y++)
		{
			game.getUnit(server.player[i].unit).removeItem(craft.recipe[argi[0]].ingr[y].type, craft.recipe[argi[0]].ingr[y].count * argi[1]);
		}
			// Remove resource
		server.player[i].addResource(craft.recipe[argi[0]].resourceBalance * argi[1]);
			// Update UI
		data << MSG_UI_UPDATEITEMLIST;
		server.sendPacket(i, data);
		data.clear();
		return true;
	}
	// =======================================================
	// =======================================================
	// Player drops an item
	if (msg == MSG_CONTROLS_DROPITEM)
	{
		// Remove item
		input >> type >> argi[0];

		if (type != "missingno")
		{
			// Remove item
			game.getUnit(server.player[i].unit).removeItem(type, argi[0]);
			// Create unit
			unitId = game.addUnit("item_a", game.getUnit(server.player[i].unit).pos + sf::Vector2f(0.00f, 30.00f));
			// Add item to unit
			game.getUnit(unitId).addItem(type, argi[0]);
		}

		// Update UI
		data << MSG_UI_UPDATEITEMLIST;
		server.sendPacket(i, data);
		data.clear();
		return true;
	}
	// =======================================================
	// =======================================================
	// Player dismantles an item
	if (msg == MSG_CONTROLS_DISMANTLE)
	{
		input >> type >> argi[0];
		dismantleData = game.getUnit(server.player[i].unit).container.get(type).dismantle;

		// Add new items
		for (int y = 0; y < dismantleData.itemCounter; y++)
		{
			game.getUnit(server.player[i].unit).addItem(dismantleData.item[y], dismantleData.amount[y] * argi[0]);
		}
		// Remove old item
		game.getUnit(server.player[i].unit).removeItem(type, argi[0]);

		// Update UI
		data << MSG_UI_UPDATEITEMLIST;
		server.sendPacket(i, data);
		data.clear();
		return true;
	}
	// =======================================================
	// =======================================================
	// Player consumes an item
	if (msg == MSG_CONTROLS_CONSUME)
	{
		input >> type >> argi[0];
		cItem* target = &game.getUnit(server.player[i].unit).container.get(type);
		consumeData = target->consume;

		// Apply consume effect
		game.getUnit(server.player[i].unit).addHealth(consumeData.healthBalance * argi[0]);
		// Dismantle on consume
		if (target->hasRef(REF_ITEM_DISMANTLE_ON_CONSUME))
		{
			// Get the data
			dismantleData = target->dismantle;
			// Add new items
			for (int y = 0; y < dismantleData.itemCounter; y++) {
				game.getUnit(server.player[i].unit).addItem(dismantleData.item[y], dismantleData.amount[y] * argi[0]);
			}
		}
		// Remove item
		game.getUnit(server.player[i].unit).removeItem(type, argi[0]);
		// Update UI
		data << MSG_UI_UPDATEITEMLIST;
		server.sendPacket(i, data);
		data.clear();

		return true;
	}
	return false;
}

bool cServer::msgControlAbility(int i, sf::Packet input)
{
	int argi[] = { 0, 0, 0, 0 };
	float argf[] = { 0.00f, 0.00f, 0.00f, 0.00f };
	bool argb[] = { false, false, false, false };
	int msg, id;
	string cmd = "";
	sf::Packet data;

	input >> msg;
	// =======================================================
	// =======================================================
	// Player uses blink item
	if (msg == MSG_CONTROLS_BLINK)
	{
		input >> argf[0] >> argf[1];

		game.getUnit(server.player[i].unit).moveTo(vec2(argf[0], argf[1]));
		return true;
	}
	// =======================================================
	// =======================================================
	// Player uses rifle item
	if (msg == MSG_CONTROLS_RIFLE)
	{
		input >> argf[0] >> argf[1];

		vec2 unitPos = game.getUnit(server.player[i].unit).pos;
		vec2 target = math.polar(unitPos, 1000.00f, math.getAngle(unitPos, vec2(argf[0], argf[1])));
		id = game.addUnit("proj_rifle", unitPos, server.player[i].unit);
		game.getUnit(id).addOrder_moveto(target);

		return true;
	}
	// =======================================================
	// =======================================================
	// Player uses an axe
	if (msg == MSG_CONTROLS_AXE || msg == MSG_CONTROLS_PICKAXE)
	{
		input >> argi[0] >> argb[1];
		id = game.getUnitId(server.player[i].unit);
		int targetId = game.getUnitId(argi[0]);
		if (id != -1 && targetId != -1) {
			cUnit* playerUnit = &game.unit[id];
			cUnit* targetUnit = &game.unit[targetId];
			// Check distance
			if (math.getDistance(playerUnit, targetUnit) <= game.getUnitInteractDistance(*playerUnit, *targetUnit))
			{
				// Units are close enough
				game.unit[id].addOrder_harvest(argi[0], true, argb[1]);
			}
			else
			{
				// Unit needs to move closer
				game.unit[id].addOrder_moveto_path(game.getUnitInteractPoint(*playerUnit, *targetUnit), !argb[1]);
				game.unit[id].addOrder_harvest(argi[0], false, argb[1]);
			}
		}
		else { cout << "[cServer::msgControlUnit / MSG_CONTROLS_HARVEST] Incorrect unit IDs!" << endl; }
		return true;
	}
	return false;
}