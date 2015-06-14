
#include "main.h"

void cItem::use()
{
	sf::Packet data;
	int unitId;
	if (!hasRef(REF_ITEM_ACTIVE) && !(hasRef(REF_ITEM_CONSUME))) { return; }
	// Remove all orders
	data << MSG_CONTROLS_STOP;
	client.sendPacket(data);
	data.clear();

	// Blink
	if (hasRef(REF_ITEM_BLINK))
	{
		data << MSG_CONTROLS_BLINK << window.getMousePos(true).x << window.getMousePos(true).y;
		client.sendPacket(data);
		data.clear();
	}
	// Rifle
	if (hasRef(REF_ITEM_RIFLE))
	{
		data << MSG_CONTROLS_RIFLE << window.getMousePos(true).x << window.getMousePos(true).y;
		client.sendPacket(data);
		data.clear();
	}
	// Axe
	if (hasRef(REF_ITEM_AXE))
	{
		unitId = game.getUnitId(window.getMousePos(true), REF_UNIT_TREE);
		if (unitId != -1)
		{
			data << MSG_CONTROLS_AXE << unitId << true;
			client.sendPacket(data);
			data.clear();
		}
	}
	// Pickaxe
	if (hasRef(REF_ITEM_PICKAXE))
	{
		unitId = game.getUnitId(window.getMousePos(true), REF_UNIT_STONE);
		if (unitId != -1)
		{
			data << MSG_CONTROLS_PICKAXE << unitId << true;
			client.sendPacket(data);
			data.clear();
		}
	}
	// Consumable
	if (hasRef(REF_ITEM_CONSUME))
	{
		data << MSG_CONTROLS_CONSUME << type << 1;
		client.sendPacket(data);
		data.clear();
	}
	// Editor items
	if (hasRef(REF_ITEM_EDITOR))
	{
		game.addUnit(type, window.getMousePos(true));
	}
}