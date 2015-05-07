
#include "main.h"

void cItem::use()
{
	sf::Packet data;
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