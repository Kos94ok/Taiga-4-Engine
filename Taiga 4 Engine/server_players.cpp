
#include "main.h"
#include "server.h"

void cServerPlayer::disconnect()
{
	connected = false;
	socket.disconnect();
	// Removing the unit
	game.removeUnit(unit);
	unit = -1;
}

void cServerPlayer::moveCamera(vec2f target)
{
	sf::Packet data;
	data << MSG_CAMERA_MOVETO << target.x << target.y;
	server.sendPacket(myId, data);
	data.clear();
}

void cServerPlayer::addResource(float d) {
	game.getUnit(unit).addResource(d);
}

void cServerPlayer::setResource(float value) {
	game.getUnit(unit).setResource(value);
}

void cServerPlayer::setHealth(float hp) {
	game.getUnit(unit).setHealth(hp);
}

void cServerPlayer::setMaxHealth(float hp) {
	game.getUnit(unit).setMaxHealth(hp);
}

void cServerPlayer::addItem(std::string type, int count) {
	game.getUnit(unit).addItem(type, count);
}

bool cServerPlayer::hasItem(std::string type, int count) {
	return game.getUnit(unit).container.getAmount(type) >= count;
}

void cServerPlayer::addBuff(int type, float duration, int power) {
	game.getUnit(unit).addBuff(type, duration, power);
}

void cServerPlayer::removeBuff(int type) {
	game.getUnit(unit).removeBuff(type);
}