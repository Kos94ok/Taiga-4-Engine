
#include "main.h"

void cEditor::enable()
{
	cout << "[CMD] Enabled editor mode." << endl;
	cout << "[CMD] Editor mode is not yet implemented, expect random bugs." << endl;
	cout << "[CMD] Saving system is not yet available, all the changes are temporary." << endl;

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
		clientUnit->container.add("campfire_full");
		targetItem = &clientUnit->container.get("campfire_full");
		targetItem->equipped = true;
	}
	ui.updateInterfaceEquipment();
	ui.updateInterfaceItemList();
	ui.updateInterfaceItemDescr();
}