
#include "main.h"
#include "item.h"
#include "client.h"
#include "window.h"
#include "game.h"
#include "visual.h"
#include "database.h"
#include "client.h"

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
		audio.playSound("rifle_shot", game.getPlayer().pos);
	}
	// Axe
	if (hasRef(REF_ITEM_AXE))
	{
		if (visual.hoveredUnit != -1 && game.getUnit(visual.hoveredUnit).hasRef(REF_UNIT_TREE))
		{
			data << MSG_CONTROLS_AXE << visual.hoveredUnit << powerLevel;
			client.sendPacket(data);
			data.clear();
		}
	}
	// Pickaxe
	if (hasRef(REF_ITEM_PICKAXE))
	{
		if (visual.hoveredUnit != -1 && game.getUnit(visual.hoveredUnit).hasRef(REF_UNIT_STONE))
		{
			data << MSG_CONTROLS_PICKAXE << visual.hoveredUnit << powerLevel;
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

void cItem::equip()
{
	sf::Packet data;
	if (!hasRef(REF_ITEM_EQUIP)) { return; }

	equipped = !equipped;
	// Flashlight
	if (hasRef(REF_ITEM_FLASHLIGHT) && equipped)
	{
		data << MSG_CONTROLS_FLASHLIGHT_ON;
		client.sendPacket(data);
		data.clear();
	}
	else if (hasRef(REF_ITEM_FLASHLIGHT) && !equipped)
	{
		data << MSG_CONTROLS_FLASHLIGHT_OFF;
		client.sendPacket(data);
		data.clear();
	}
	// Torch
	if (hasRef(REF_ITEM_TORCH) && equipped)
	{
		data << MSG_CONTROLS_TORCH_ON;
		client.sendPacket(data);
		data.clear();
	}
	else if (hasRef(REF_ITEM_TORCH) && !equipped)
	{
		data << MSG_CONTROLS_TORCH_OFF;
		client.sendPacket(data);
		data.clear();
	}
}