
#include "main.h"
#include "unit.h"
#include "craft.h"
#include "UI.h"
#include "server.h"
#include "math.h"

bool cCharacter::animAvailable(int animType)
{
	return (animData[animType].side.tex != -1);
}

void cUnit::addResource(float d) { setResource(resource + d); }

void cUnit::setResource(float value)
{
	resource = value;

	craft.checkActiveRecipe();
	ui.updateInterfaceItemList();

	if (core.serverMode || core.localServer)
	{
		sf::Packet data;
		data << MSG_UNIT_SETRESOURCE << globalId << value;
		server.sendPacket(PLAYERS_REMOTE, data);
	}
}

void cUnit::moveTo(vec2 newPos)
{
	pos = newPos;

	if (core.serverMode || core.localServer)
	{
		sf::Packet data;
		data << MSG_UNIT_MOVETO << globalId << pos.x << pos.y;
		server.sendPacket(PLAYERS_REMOTE, data);
	}
}

void cUnit::rotateTo(float newAngle)
{
	targetFacingAngle = newAngle;

	if (core.serverMode || core.localServer)
	{
		sf::Packet data;
		data << MSG_UNIT_ROTATETO << globalId << newAngle;
		server.sendPacket(PLAYERS_REMOTE, data);
	}
}

void cUnit::addHealth(float d) { setHealth(health + d); }
void cUnit::addMaxHealth(float d) { setMaxHealth(maxHealth + d); }

void cUnit::setHealth(float hp)
{
	health = hp;

	ui.updateInterface();

	if (core.serverMode || core.localServer)
	{
		sf::Packet data;
		data << MSG_UNIT_SETHEALTH << globalId << hp;
		server.sendPacket(PLAYERS_REMOTE, data);
	}
}

void cUnit::setMaxHealth(float hp)
{
	maxHealth = hp;

	ui.updateInterface();

	if (core.serverMode || core.localServer)
	{
		sf::Packet data;
		data << MSG_UNIT_SETMAXHEALTH << globalId << hp;
		server.sendPacket(PLAYERS_REMOTE, data);
	}
}

void cUnit::addItem(string type, int count)
{
	container.add(type, count);
	container.sort(util.getInventorySortingType());
	// Broadcast the data
	if (core.serverMode || core.localServer)
	{
		sf::Packet data;
		data << MSG_UNIT_ADDITEM << globalId << type << count;
		server.sendPacket(PLAYERS_REMOTE, data);
		data.clear();
	}
}

void cUnit::removeItem(string type, int count)
{
	container.remove(type, count);
	// Broadcast the data
	if (core.serverMode || core.localServer)
	{
		sf::Packet data;
		data << MSG_UNIT_REMOVEITEM << globalId << type << count;
		server.sendPacket(PLAYERS_REMOTE, data);
		data.clear();
	}
}

void cUnit::removeItem(int id, int count)
{
	container.remove(id, count);
	// Broadcast the data
	if (core.serverMode || core.localServer)
	{
		sf::Packet data;
		data << MSG_UNIT_REMOVEITEM << globalId << container.get(id).type << count;
		server.sendPacket(PLAYERS_REMOTE, data);
		data.clear();
	}
}

void cUnit::addBuff(int type, float duration, int power)
{
	buff.add(type, duration, power);
	// Broadcast the data
	if (core.serverMode || core.localServer)
	{
		sf::Packet data;
		data << MSG_UNIT_ADDBUFF << globalId << type << duration << power;
		server.sendPacket(PLAYERS_REMOTE, data);
		data.clear();
	}
}

void cUnit::removeBuff(int type)
{
	buff.remove(type);
	// Broadcast the data
	if (core.serverMode || core.localServer)
	{
		sf::Packet data;
		data << MSG_UNIT_REMOVEBUFF << globalId << type;
		server.sendPacket(PLAYERS_REMOTE, data);
		data.clear();
	}
}

bool cUnit::hasBuff(int type)
{
	return buff.getPower(type) > 0;
}

void cUnit::setLifeTimer(float time)
{
	lifeTimer.enabled = true;
	lifeTimer.time = time;
}

void cUnit::resetLifeTimer()
{
	lifeTimer.enabled = false;
}

void cUnit::updateFacing()
{
	if (orderCounter == 0) { return; }
	float angle;

	if (order[0].type == ORDER_MOVETO) {
		angle = math.getAngle(pos.x, pos.y, order[0].targetPos.x, order[0].targetPos.y);
	}
	else {
		angle = math.getAngle(pos.x, pos.y, game.getUnit(order[0].targetObject).pos.x, game.getUnit(order[0].targetObject).pos.y);
	}
	rotateTo(math.convertAngle(angle));
}

void cUnit::updateAction()
{
	if (orderCounter == 0 || actionTimer > 0.00f) { return; }
	
	if (order[0].type == ORDER_PICKUP) { actionTimer = 0.00f; }
	else if (order[0].type == ORDER_HARVEST) { actionTimer = 3.00f; }
	else if (order[0].type == ORDER_DEATH)
	{
		if (animAvailable(ANIM_DEATH))
		{
			// Force the animation
			anim.play(ANIM_DEATH);
			actionTimer = (getCurrentAnimDirection().data.frameCount - 1) * getCurrentAnimDirection().data.frameDelay;
		}
	}
}

void cUnit::updateAnimation()
{
	if (orderCounter == 0) { anim.play(ANIM_IDLE); }
	else
	{
		if (order[0].type == ORDER_MOVETO && animAvailable(ANIM_MOVE)) { anim.play(ANIM_MOVE); }
		else if (order[0].type == ORDER_PICKUP && animAvailable(ANIM_PICKUP)) { anim.play(ANIM_PICKUP); }
		else if (order[0].type == ORDER_HARVEST && animAvailable(ANIM_WORK)) { anim.play(ANIM_WORK); }
		else if (order[0].type == ORDER_DEATH && animAvailable(ANIM_DEATH)) { anim.play(ANIM_DEATH); }
	}
}