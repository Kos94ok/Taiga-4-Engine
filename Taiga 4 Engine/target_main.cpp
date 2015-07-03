
#include "target.h"
#include "UI.h"
#include "game.h"
#include "camera.h"
#include "script.h"
#include "client.h"

void cTarget::enable_forButton(int elementId)
{
	reset();
	mode = MODE_TARGET_BUTTON;
	// Checking the argument
	int id = ui.getElementId(elementId);
	string type;
	if (id == -1) { console.error << "[cTarget::enable_forButton] Incorrect element id!" << endl; return; }

	// Activating
	if (!active) {
		active = true;
	}
	forButton = elementId;
}

void cTarget::enable_building(string type, int ref)
{
	reset();
	mode = MODE_TARGET_BUILDING;
	// Adding the new unit
	game.access.lock();
	game.addUnit(type, vec2f(0.00f, 0.00f), -1, -1, false, ID_TEMP);
	game.getUnit(ID_TEMP).addRef(REF_UNIT_TARGET);
	game.getUnit(ID_TEMP).addRef(REF_UNIT_NOSELECTION);
	game.access.unlock();

	// Activating the script
	if (!activeBuild) {
		script.execute(cScript::unit_bindToMouse, to_string(REF_UNIT_TARGET));
	}
	active = true;
	activeBuild = true;
}

void cTarget::apply()
{
	sf::Packet data;
	if (mode == MODE_TARGET_BUTTON)
	{
		int id = ui.getElementId(forButton);
		if (id != -1) {
			ui.element[id].button.callbackLeft(forButton);
		}
	}
	else if (mode == MODE_TARGET_BUILDING)
	{
		data << MSG_CONTROLS_BUILD << 
	}
	reset();
}

void cTarget::reset()
{
	mode = MODE_NONE;
	active = false;
	activeBuild = false;

	cUnit* oldUnit = &game.getUnitByRef(REF_UNIT_TARGET);
	if (oldUnit->type != "missingno") {
		game.removeUnit(oldUnit->globalId, false);
	}
}