
#include "main.h"

int cUnit::addOrder_moveto(sf::Vector2f target, bool overwrite)
{
	// If unit can not move, then ignore
	if (movementSpeed <= 0.00f) { return -1; }
	// If the target is too close, then ignore
	if (math.getDistance(pos.x, pos.y, target.x, target.y) < movementSpeed / 10) { return -1; }

	// Clearing the order list
	if (overwrite) { orderCounter = 0; }
	// Reset the action timer
	if (orderCounter == 0) { actionTimer = 0.00f; }
	// Adding new order
	order[orderCounter].type = ORDER_MOVETO;
	order[orderCounter].targetPos = target;
	orderCounter += 1;

	// Update info
	updateFacing();
	updateAction();
	updateAnimation();

	// Server
	sf::Packet data;
	data << MSG_ORDER_MOVETO << globalId << target.x << target.y << overwrite;
	server.sendPacket(PLAYERS_REMOTE, data);

	return orderCounter - 1;
}

int cUnit::addOrder_moveto_path(sf::Vector2f target, bool overwrite)
{
	// If unit can not move, then ignore
	if (movementSpeed <= 0.00f) { return -1; }
	// If target is too close, then ignore
	if (math.getDistance(pos, target) < 10.00f) { return -1; }
	// If clicked to the restricted area, then... you know
	vec2i targetChunk = world.getChunkInPos(target);
	//if (world.map[targetChunk.x][targetChunk.y].type == CHUNK_UNDEFINED) { return -1; }

	if (overwrite) { orderCounter = 0; actionTimer = 0.00f; }

	// Calculating path
	path.calculate(pos, target, collisionDistance, globalId);
	// Adding orders
	for (int i = 0; i < path.waypointCounter; i++)
	{
		order[orderCounter].type = ORDER_MOVETO;
		order[orderCounter].targetPos = path.waypoint[i];
		orderCounter += 1;

		// Server
		/*sf::Packet data;
		data << MSG_ORDER_MOVETO << globalId << path.waypoint[i].x << path.waypoint[i].y << (overwrite && i == 0);
		server.sendPacket(PLAYERS_REMOTE, data);*/
	}
	// Server
	sf::Packet data;
	data << MSG_ORDER_MOVETOPATH << globalId << target.x << target.y << overwrite;
	server.sendPacket(PLAYERS_REMOTE, data);
	// Update info
	updateFacing();
	updateAction();
	updateAnimation();

	return orderCounter - 1;
}

int cUnit::addOrder_pickup(int target, bool overwrite)
{
	// Clearing the order list
	if (overwrite) { orderCounter = 0; }
	// Reset the action timer
	if (orderCounter == 0) { actionTimer = 0.00f; }
	// Adding new order
	order[orderCounter].type = ORDER_PICKUP;
	order[orderCounter].targetObject = target;
	orderCounter += 1;

	// Update info
	updateFacing();
	updateAction();
	updateAnimation();

	// Server
	sf::Packet data;
	data << MSG_ORDER_PICKUP << globalId << target << overwrite;
	server.sendPacket(PLAYERS_REMOTE, data);

	return orderCounter - 1;
}

int cUnit::addOrder_harvest(int target, bool overwrite)
{
	// Clearing the order list
	if (overwrite) { orderCounter = 0; }
	// Reset the action timer
	if (orderCounter == 0) { actionTimer = 0.00f; }
	// Adding new order
	order[orderCounter].type = ORDER_HARVEST;
	order[orderCounter].targetObject = target;
	orderCounter += 1;

	// Update info
	updateFacing();
	updateAction();
	updateAnimation();

	// Server
	sf::Packet data;
	data << MSG_ORDER_HARVEST << globalId << target << overwrite;
	server.sendPacket(PLAYERS_REMOTE, data);

	return orderCounter - 1;
}

void cUnit::removeOrder(int id)
{
	for (int i = id; i < orderCounter - 1; i++)
	{
		order[i] = order[i + 1];
	}
	orderCounter -= 1;
	if (id == 0) { actionTimer = 0.00f; }
}

void cUnit::removeAllOrders()
{
	orderCounter = 0;
	actionTimer = 0.00f;

	updateFacing();
	updateAction();
	updateAnimation();

	sf::Packet data;
	data << MSG_ORDER_STOP << globalId;
	server.sendPacket(PLAYERS_REMOTE, data);
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

void cUnit::updateFacing()
{
	if (orderCounter == 0) { return; }
	float angle;

	angle = math.getAngle(pos.x, pos.y, order[0].targetPos.x, order[0].targetPos.y);
	facingAngle = math.convertAngle(angle);
}

void cUnit::updateAction()
{
	if (orderCounter == 0 || actionTimer > 0.00f) { return; }
	
	if (order[0].type == ORDER_PICKUP) { actionTimer = 0.00f; }
	else if (order[0].type == ORDER_HARVEST) { actionTimer = 3.00f; }
}

void cUnit::updateAnimation()
{
	if (orderCounter == 0) { anim.play(ANIM_IDLE); }
	else
	{
		if (order[0].type == ORDER_MOVETO) { anim.play(ANIM_MOVE); }
		else if (order[0].type == ORDER_PICKUP) { anim.play(ANIM_PICKUP); }
		else if (order[0].type == ORDER_HARVEST) { anim.play(ANIM_WORK); }
	}
}