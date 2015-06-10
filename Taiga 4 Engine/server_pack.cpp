
#include "main.h"

sf::Packet cServer::packUnitData(int id, int variation)
{
	sf::Packet data;
	id = game.getUnitId(id);
	if (id == -1) { console << "[ERROR - cServer::packUnitData] Can't find the unit!" << endl; return data; }
	cUnit* unit = &game.unit[id];
	data << MSG_UNIT_ADD << unit->globalId << unit->type << unit->pos.x << unit->pos.y << unit->owner << variation;
	return data;
}