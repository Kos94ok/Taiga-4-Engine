
#include "main.h"

// Copy unit from database
int cGame::addUnit(string type, sf::Vector2f pos, int owner, int variation, bool sendData)
{
	access.lock();
	if (!sendData || core.serverMode || core.localServer)
	{
		if (variation == 0) { type += "a"; }
		if (variation == 1) { type += "b"; }
		if (variation == 2) { type += "c"; }
		if (variation == 3) { type += "d"; }
		// Apply
		if (unitCounter < LIMIT_UNIT - 1)
		{
			unit[unitCounter] = database.getUnit(type);
			unit[unitCounter].pos = pos;
			unit[unitCounter].chunkPos = world.getChunkInPos(pos);
			unit[unitCounter].owner = owner;
			if (core.serverMode || core.localServer) { unit[unitCounter].globalId = unitGlobalCounter++; }
			// Playing the sound
			if (unit[unitCounter].sound.idle.name != "") {
				audio.playSound(cSoundQueue(unit[unitCounter].sound.idle, unit[unitCounter].globalId, true));
			}
			// Incrementing
			unitCounter += 1;
		}
		else { console.error << "[ERROR] Unit limit reached!" << "\n"; }
	}

	// Server
	if (sendData && (core.serverMode || core.localServer))
	{
		sf::Packet data;
		data << MSG_UNIT_ADD << unitGlobalCounter - 1 << type << pos.x << pos.y << owner << variation;
		server.sendPacket(PLAYERS_REMOTE, data);
	}
	access.unlock();
	return game.unitGlobalCounter - 1;
}

// Remove unit by global id
void cGame::removeUnit(int id, bool sendData)
{
	//access.lock();
	if (!sendData || core.serverMode || core.localServer)
	{
		// Server
		if (sendData && (core.serverMode || core.localServer))
		{
			sf::Packet data;
			data << MSG_UNIT_REMOVE << id;
			server.sendPacket(PLAYERS_REMOTE, data);
		}

		// Apply
		id = getUnitId(id);
		if (id != -1)
		{
			for (int i = id; i < unitCounter - 1; i++)
			{
				unit[i] = unit[i + 1];
			}
			unitCounter -= 1;
		}
	}
	//access.unlock();
}

// Kill unit
void cGame::killUnit(int id)
{
	game.getUnit(id).addOrder_death();
}

// Damage unit
void cGame::damageUnit(int id, float damage)
{
	int unitId = game.getUnitId(id);
	game.unit[unitId].addHealth(-damage);
	if (game.unit[unitId].health <= 0.00f)
	{
		killUnit(id);
	}
}

// Remove all units
void cGame::clearUnits()
{
	unitCounter = 0;
}

// Using items
void cGame::useItem(int id)
{
	unit[getUnitId(client.unit)].container.get(id).use();
}

// Quick create drop
int cGame::createDrop(vec2f pos, vector<cItemDrop> itemList)
{
	if (itemList.size() == 0) { console << "[cGame::createDrop] Empty item list!" << endl; return -1; }

	access.lock();
	int unitId = addUnit("item_a", pos);
	for (int i = 0; i < (int)itemList.size(); i++)
	{
		if (itemList[i].type.length() > 0)
		{
			getUnit(unitId).addItem(itemList[i].type, itemList[i].count);
		}
	}
	access.unlock();

	return unitId;
}

int cGame::createDrop(vec2f pos, cItemDrop itemA, cItemDrop itemB, cItemDrop itemC, cItemDrop itemD)
{
	vector<cItemDrop> itemList;
	itemList.push_back(itemA);
	itemList.push_back(itemB);
	itemList.push_back(itemC);
	itemList.push_back(itemD);
	return createDrop(pos, itemList);
}

// Manipulate game world data
void cGame::setTimeOfDay(float time)
{
	timeOfDay = time;

	// Server
	if (core.serverMode || core.localServer)
	{
		sf::Packet data;
		data << MSG_GAME_TIME << timeOfDay;
		server.sendPacket(PLAYERS_REMOTE, data);
		data.clear();
	}
}

void cGame::setAmbientLight(float light)
{
	ambientLight = light;
	if (ambientLight > ambientLightMax) { ambientLight = ambientLightMax; }
	else if (ambientLight < ambientLightMin) { ambientLight = ambientLightMin; }

	// Server
	if (core.serverMode || core.localServer)
	{
		sf::Packet data;
		data << MSG_GAME_LIGHT << ambientLight;
		server.sendPacket(PLAYERS_REMOTE, data);
		data.clear();
	}
}