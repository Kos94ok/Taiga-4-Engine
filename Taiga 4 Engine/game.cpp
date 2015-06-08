
#include "main.h"

// Copy unit from database
int cGame::addUnit(string type, sf::Vector2f pos, int owner, int variation, bool sendData)
{
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
			unit[unitCounter].globalId = unitGlobalCounter++;
			unit[unitCounter].pos = pos;
			unit[unitCounter].chunkPos = world.getChunkInPos(pos);
			unit[unitCounter].owner = owner;
			unitCounter += 1;
		}
		else { cout << "[ERROR] Unit limit reached!" << "\n"; }
	}

	// Server
	if (sendData && (core.serverMode || core.localServer))
	{
		sf::Packet data;
		data << MSG_UNIT_ADD << unitGlobalCounter - 1 << type << pos.x << pos.y << owner << variation;
		server.sendPacket(PLAYERS_REMOTE, data);
	}
	return game.unitGlobalCounter - 1;
}

// Remove unit by global id
void cGame::removeUnit(int id, bool sendData)
{
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

// Convert global id to local id
int cGame::getUnitId(int globalId)
{
	for (int i = 0; i < unitCounter; i++)
	{
		if (unit[i].globalId == globalId) { return i; }
	}
	return -1;
}

// Convert world location to local id
int cGame::getUnitId(sf::Vector2f pos, int refFilter)
{
	cUnit unit = getUnit(pos, refFilter);
	if (unit.type == "missingno") { return -1; }
	return unit.globalId;
}

// Get unit
cUnit& cGame::getUnit(int id)
{
	id = getUnitId(id);
	if (id != -1) {
		return unit[id];
	}
	cout << "[cGame::getUnit] Can't find the unit " << id << "!" << "\n";
	return database.unit[0];
}

// Convert world location to unit
cUnit& cGame::getUnit(vec2f pos, int refFilter)
{
	for (int i = 0; i < unitCounter; i++)
	{
		if (math.getDistance(unit[i].pos, pos) < unit[i].interactDistance && unit[i].hasRef(refFilter)) {
			return unit[i];
		}
	}
	return database.unit[0];
}

// Convert world location to unit
cUnit& cGame::getUnitPhys(vec2f &pos, int refFilter)
{
	for (int i = 0; i < unitCounter; i++)
	{
		if (math.getDistance(unit[i].pos, pos) < unit[i].collisionDistance && unit[i].hasRef(refFilter)) {
			return unit[i];
		}
	}
	return database.unit[0];
}

// Get the distance required for interact
float cGame::getUnitInteractDistance(cUnit unitA, cUnit unitB) {
	return (unitA.interactDistance + unitB.interactDistance);
}

// Get the distance required for collision
float cGame::getUnitCollisionDistance(cUnit *unitA, cUnit *unitB) {
	return (unitA->collisionDistance + unitB->collisionDistance);
}

// Get closest interact point
vec2f cGame::getUnitInteractPoint(cUnit unitA, cUnit unitB) {
	return math.polar(unitB.pos, unitA.interactDistance + unitB.interactDistance, math.getAngle(&unitB, &unitA));
}

// Time checks
bool cGame::isDay() { return (timeOfDay > 6.00f && timeOfDay < 18.00f); }
bool cGame::isNight() { return !isDay(); }
bool cGame::lightens() { return (timeOfDay > 4.00f && timeOfDay < 16.00f); }
bool cGame::darkens() { return !lightens(); }

// Using items
void cGame::useItem(int id)
{
	unit[getUnitId(client.unit)].container.get(id).use();
}