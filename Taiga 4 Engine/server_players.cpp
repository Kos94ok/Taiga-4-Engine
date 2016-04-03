
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

void cServerPlayer::addHealth(float val) {
	game.getUnit(unit).addHealth(val);
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

void cServerPlayer::addBuff(int type, float duration, int power, int owner) {
	game.getUnit(unit).addBuff(type, duration, power, owner);
}

void cServerPlayer::addBuff(cBuff value) {
	game.getUnit(unit).addBuff(value);
}

void cServerPlayer::removeBuff(int type) {
	game.getUnit(unit).removeBuff(type);
}

//==================================
// Cold and hunger
//==================================
void cServerPlayer::addCold(float value, bool local) {
	setCold(statCold + value, local);
}

void cServerPlayer::setCold(float value, bool local)
{
	float damage = 0.00f;
	statCold = value;
	if (statCold < getMinCold()) { damage = abs(getMinCold() - statCold); statCold = getMinCold(); }
	else if (statCold > getMaxCold()) { statCold = getMaxCold(); }

	if (!local)
	{
		sf::Packet data;
		data << MSG_PLAYER_COLD << statCold;
		server.sendPacket(myId, data);
		data.clear();
		if (damage > 0.00f)
			game.getUnit(unit).addHealth(-damage);
	}
}

void cServerPlayer::addHunger(float value, bool local) {
	setHunger(statHunger + value, local);
}

void cServerPlayer::setHunger(float value, bool local)
{
	float damage = 0.00f;
	statHunger = value;
	if (statHunger < getMinHunger()) { damage = abs(getMinHunger() - statHunger); statHunger = getMinHunger(); }
	else if (statHunger > getMaxHunger()) { statHunger = getMaxHunger(); }

	if (!local)
	{
		sf::Packet data;
		data << MSG_PLAYER_HUNGER << statHunger;
		server.sendPacket(myId, data);
		if (damage > 0.00f)
			game.getUnit(unit).addHealth(-damage);
	}
}

float cServerPlayer::getMinCold()
{
	return value.getMinColdLocal();
}

float cServerPlayer::getMaxCold()
{
	return value.getMaxColdLocal();
}

float cServerPlayer::getMinHunger()
{
	return value.getMinHungerLocal();
}

float cServerPlayer::getMaxHunger()
{
	return value.getMaxHungerLocal();
}