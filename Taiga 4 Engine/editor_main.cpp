
#include "main.h"

void cEditor::enable()
{
	cout << "[CMD] Enabled editor mode." << "\n";
	core.editorMode = true;
}

void cEditor::initialize()
{
	cUnit* clientUnit = &game.getUnit(client.unit);
	cItem* targetItem;
	if (clientUnit->type != "missingno")
	{
		clientUnit->container.add("tree_basic_a");
		targetItem = &clientUnit->container.get("tree_basic_a");
		targetItem->equipped = true;
		clientUnit->container.add("tree_basic_b");
		targetItem = &clientUnit->container.get("tree_basic_b");
		targetItem->equipped = true;
		clientUnit->container.add("tree_basic_c");
		targetItem = &clientUnit->container.get("tree_basic_c");
		targetItem->equipped = true;
		clientUnit->container.add("tree_basic_d");
		targetItem = &clientUnit->container.get("tree_basic_d");
		targetItem->equipped = true;
		clientUnit->container.add("campfire_full");
		targetItem = &clientUnit->container.get("campfire_full");
		targetItem->equipped = true;
	}
	ui.updateInterfaceEquipment();
	ui.updateInterfaceItemList();
	ui.updateInterfaceItemDescr();
}