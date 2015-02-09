
#include "main.h"

void cUI::updateInterface()
{
	// Health bar
	int search = findByRef(REF_UI_HEALTHBAR);
	if (client.connected && search != -1 && client.unit != -1)
	{
		int id = game.getUnitId(client.unit);
		if (id != -1) {
			ui.element[search].bar.value = game.unit[id].displayHealth;
			ui.element[search].bar.maxValue = game.unit[id].maxHealth;
		}
	}
}

void cUI::updateInterfaceItemDescr()
{
	if (!invOpened) { return; }
	access.lock();

	float resModX = (float)camera.res.x / 1280.00f;
	float resModY = (float)camera.res.y / 800.00f;

	ui.removeElementsByRef(REF_UI_INVENTORY_DESCR);
	int hoverId = -1;
	for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
	{
		if (element[i].isValid && element[i].isHovered && (element[i].button.action == "invItem_list" || element[i].button.action == "invItem_craftRemove"
			|| element[i].button.action == "invItem_craftResult"))
		{
			stringstream(element[i].button.args[0]) >> hoverId;
			i = LIMIT_UI_ELEMENTS;
		}
	}

	cItem item = game.unit[game.getUnitId(client.unit)].container.get(hoverId);
	if (item.type == "missingno") { item = craft.cont.get(hoverId); }
	if (item.type == "missingno") { item = craft.resultCont.get(hoverId); }

	if (item.type != "missingno")
	{
		string txt = item.displayName;
		if (core.debugMode) { txt += " (" + item.type + ")"; }
		int id = ui.addElement("text", sf::Vector2f(math.round(700.00f * resModX + 20.00f), 170.00f + 0.00f));
		ui.element[ui.getElementId(id)].setText(txt);
		ui.element[ui.getElementId(id)].addRef(REF_UI_INVENTORY);
		ui.element[ui.getElementId(id)].addRef(REF_UI_INVENTORY_DESCR);
		ui.element[ui.getElementId(id)].ignoreOrigin = true;
		ui.element[ui.getElementId(id)].textColor = sf::Color(255, 255, 255);
		ui.element[ui.getElementId(id)].textSize = 24;
		ui.element[ui.getElementId(id)].textFont = FONT_DESCR;
		for (int i = 0; i < LIMIT_DESCR; i++)
		{
			txt = item.description[i];
			if (txt.length() > 0)
			{
				id = ui.addElement("text", sf::Vector2f(math.round(700.00f * resModX + 10.00f), 170.00f + 32.00f + 22.00f * i));
				ui.element[ui.getElementId(id)].setText(txt);
				ui.element[ui.getElementId(id)].addRef(REF_UI_INVENTORY);
				ui.element[ui.getElementId(id)].addRef(REF_UI_INVENTORY_DESCR);
				ui.element[ui.getElementId(id)].ignoreOrigin = true;
				ui.element[ui.getElementId(id)].textColor = sf::Color(255, 255, 255);
				ui.element[ui.getElementId(id)].textSize = 16;
				ui.element[ui.getElementId(id)].textFont = FONT_DESCR;
			}
		}
	}
	access.unlock();
}

cItemContainer cont;
void cUI::updateInterfaceItemList()
{
	if (!invOpened) { return; }
	access.lock();
	int id;
	string name, text;
	float resModX = (float)camera.res.x / 1280.00f;
	float resModY = (float)camera.res.y / 800.00f;
	ui.removeElementsByRef(REF_UI_INVENTORY_ITEM);
	ui.removeElementsByRef(REF_UI_INVENTORY_WEIGHT);

	// Item list active category
	if (inventoryCategory == CATEGORY_ALL) { text = "All items:"; }
	else if (inventoryCategory == CATEGORY_EQUIPPED) { text = "Equipped:"; }
	else if (inventoryCategory == CATEGORY_WEAPONS) { text = "Weapons / Tools:"; }
	else if (inventoryCategory == CATEGORY_ARMOR) { text = "Armor / Clothes:"; }
	else if (inventoryCategory == CATEGORY_MATERIALS) { text = "Materials:"; }
	else if (inventoryCategory == CATEGORY_CONSUMABLES) { text = "Consumables:"; }
	else if (inventoryCategory == CATEGORY_OTHER) { text = "Other:"; }
	id = createText(sf::Vector2f(90.00f, 180.00f), text, "", REF_UI_INVENTORY);
	element[getElementId(id)].addRef(REF_UI_INVENTORY_WEIGHT);

	int itemsFound = 0;
	// Inventory item list
	if (game.getUnitId(client.unit) != -1)
	{
		cont = game.unit[game.getUnitId(client.unit)].container;
		for (int i = 0; i < cont.itemCounter; i++)
		{
			if (((inventoryCategory == CATEGORY_ALL || cont.item[i].category == inventoryCategory) && !cont.item[i].equipped
				|| (inventoryCategory == CATEGORY_EQUIPPED && cont.item[i].equipped))
				&& (core.debugMode || cont.item[i].type != "missingno"))
			{
				// Main button
				id = ui.addElement("button", sf::Vector2f(70.00f, 240.00f + 26.00f * itemsFound));
				ui.element[ui.getElementId(id)].size = sf::Vector2f(math.round(550.00f * resModX - 60.00f - 20.00f), 26.00f);
				name = "- " + cont.item[i].displayName + " x" + to_string(cont.amount[i]);
				ui.element[ui.getElementId(id)].setText(name);
				ui.element[ui.getElementId(id)].textSize = 24;
				ui.element[ui.getElementId(id)].textOffset.y = -3.00f;
				ui.element[ui.getElementId(id)].texture = visual.addTexture("alpha.png");
				ui.element[ui.getElementId(id)].textureHovered = visual.addTexture("black.png");
				ui.element[ui.getElementId(id)].textColorHover = sf::Color(255, 127, 0);
				ui.element[ui.getElementId(id)].ignoreOrigin = true;
				ui.element[ui.getElementId(id)].addRef(REF_UI_INVENTORY);
				ui.element[ui.getElementId(id)].addRef(REF_UI_INVENTORY_ITEM);
				ui.element[ui.getElementId(id)].button.action = "invItem_list";
				ui.element[ui.getElementId(id)].button.args[0] = to_string(cont.item[i].globalId);
				ui.element[ui.getElementId(id)].hoverAlpha = 0;
				// Attribute 0
				/*id = ui.addElement(ui.element[ui.getElementId(id)], sf::Vector2f(70.00f, 240.00f + 26.00f * i));
				ui.element[ui.getElementId(id)].button.action = "";
				ui.element[ui.getElementId(id)].textOffset.x = 320.00f * resModX - 20.00f;
				ui.element[ui.getElementId(id)].setText("X smth");
				ui.element[ui.getElementId(id)].textureHovered = visual.addTexture("alpha.png");*/
				// Attribute 1
				id = ui.addElement(ui.element[ui.getElementId(id)], sf::Vector2f(70.00f, 240.00f + 26.00f * itemsFound));
				ui.element[ui.getElementId(id)].button.action = "";
				ui.element[ui.getElementId(id)].textOffset.x = 420.00f * resModX - 20.00f;
				ui.element[ui.getElementId(id)].setText(to_string(math.round(cont.item[i].weight * cont.amount[i])) + " kg");
				ui.element[ui.getElementId(id)].textureHovered = visual.addTexture("alpha.png");
				itemsFound += 1;
			}
		}
	}
	// Inventory craft list
	cont = craft.cont;
	int curRecipe = craft.getActiveRecipeId();
	int itemsNeeded = 0;
	//craft.recipe[curRecipe].
	for (int i = 0; i < cont.itemCounter; i++)
	{
		// Main button
		id = ui.addElement("button", sf::Vector2f(math.round(700.00f * resModX + 10.00f), math.round(500.00f * resModY + 10.00f + 26.00f * i)));
		ui.element[ui.getElementId(id)].size = sf::Vector2f((550.00f * resModX - 60.00f - 20.00f), 26.00f);
		ui.element[ui.getElementId(id)].textSize = 24;
		if (curRecipe != -1) {
			// Get amount of items needed for the recipe
			for (int y = 0; y < craft.recipe[curRecipe].ingrCount; y++) {
				if (craft.recipe[curRecipe].ingr[y].type == cont.item[i].type) {
					itemsNeeded = craft.recipe[curRecipe].ingr[y].count;
					y = craft.recipe[curRecipe].ingrCount;
				}
			}
			name = "- " + cont.item[i].displayName + " x" + to_string(cont.amount[i]) + " / " + to_string(itemsNeeded);
		}
		else {
			name = "- " + cont.item[i].displayName + " x" + to_string(cont.amount[i]) + " / ?";
		}
		ui.element[ui.getElementId(id)].setText(name);
		ui.element[ui.getElementId(id)].texture = visual.addTexture("alpha.png");
		ui.element[ui.getElementId(id)].textureHovered = visual.addTexture("black.png");
		ui.element[ui.getElementId(id)].textColorHover = sf::Color(255, 127, 0);
		ui.element[ui.getElementId(id)].textOffset.y = -3.00f;
		ui.element[ui.getElementId(id)].ignoreOrigin = true;
		ui.element[ui.getElementId(id)].addRef(REF_UI_INVENTORY);
		ui.element[ui.getElementId(id)].addRef(REF_UI_INVENTORY_ITEM);
		ui.element[ui.getElementId(id)].button.action = "invItem_craftRemove";
		ui.element[ui.getElementId(id)].button.args[0] = to_string(cont.item[i].globalId);
		ui.element[ui.getElementId(id)].hoverAlpha = 0;
	}

	// Amount of resource needed
	if (craft.getActiveRecipeId() != -1)
	{
		float resBalance = craft.recipe[craft.getActiveRecipeId()].resourceBalance;
		if (resBalance != 0.00f)
		{
			id = ui.addElement(ui.element[ui.getElementId(id)], sf::Vector2f(math.round(700.00f * resModX + 10.00f), math.round(500.00f * resModY + 10.00f + 26.00f * cont.itemCounter)));
			ui.element[ui.getElementId(id)].button.action = "-";
			if (resBalance > 0.00f)
			{
				name = "- Resource (+" + to_string(math.round(resBalance * craft.getActiveRecipeRepeats()))
					+ " / " + to_string(math.round(resBalance)) + ")";
				ui.element[ui.getElementId(id)].textColor = sf::Color(100, 255, 100);
			}
			else if (resBalance < 0.00f)
			{
				name = "- Resource (" + to_string(math.round(-resBalance * craft.getActiveRecipeRepeats()))
					+ " / " + to_string(math.round(-resBalance)) + ")";
				if (game.getUnit(client.unit).resource < -resBalance) {
					ui.element[ui.getElementId(id)].textColor = sf::Color(255, 100, 100);
				}
			}
			ui.element[ui.getElementId(id)].setText(name);
		}
	}
	// Inventory craft result
	cont = craft.resultCont;

	if (cont.itemCounter > 0 && cont.item[0].displayName.length() > 0)
	{
		id = ui.addElement("button", sf::Vector2f(math.round(700.00f * resModX + 10.00f), math.round(750.00f * resModY - 50.00f)));
		ui.element[ui.getElementId(id)].size = sf::Vector2f((550.00f * resModX - 60.00f - 20.00f), 26.00f);
		ui.element[ui.getElementId(id)].textSize = 24;
		name = "- " + cont.item[0].displayName + " x" + to_string(cont.amount[0]);
		ui.element[ui.getElementId(id)].setText(name);
		ui.element[ui.getElementId(id)].texture = visual.addTexture("alpha.png");
		ui.element[ui.getElementId(id)].textureHovered = visual.addTexture("black.png");
		ui.element[ui.getElementId(id)].textColorHover = sf::Color(255, 127, 0);
		ui.element[ui.getElementId(id)].textOffset.y = -3.00f;
		ui.element[ui.getElementId(id)].ignoreOrigin = true;
		ui.element[ui.getElementId(id)].addRef(REF_UI_INVENTORY);
		ui.element[ui.getElementId(id)].addRef(REF_UI_INVENTORY_ITEM);
		ui.element[ui.getElementId(id)].button.action = "invItem_craftResult";
		ui.element[ui.getElementId(id)].button.args[0] = to_string(cont.item[0].globalId);
		ui.element[ui.getElementId(id)].hoverAlpha = 0;
	}

	// Resource (total)
	text = "Resource: " + to_string(math.round(game.getUnit(client.unit).resource));
	id = ui.createText(sf::Vector2f(650 * resModX, 40.00f), text, "Random useful stuff. Effectively - your \"money\"", REF_UI_INVENTORY);
	ui.element[ui.getElementId(id)].addRef(REF_UI_INVENTORY_WEIGHT);

	// Total weight
	float totalWeight = 0.00f;
		// Player container
	for (int i = 0; i < game.unit[game.getUnitId(client.unit)].container.itemCounter; i++) {
		totalWeight += game.unit[game.getUnitId(client.unit)].container.item[i].weight * game.unit[game.getUnitId(client.unit)].container.amount[i];
	}
		// Craft container
	for (int i = 0; i < craft.cont.itemCounter; i++) {
		totalWeight += craft.cont.item[i].weight * craft.cont.amount[i];
	}
		// Displaying
	text = "Total weight: " + to_string(math.round(totalWeight)) + " / 55 kg";
	id = ui.createText(sf::Vector2f(950 * resModX, 40.00f), text, "Total item weight your character can carry", REF_UI_INVENTORY);
	ui.element[ui.getElementId(id)].addRef(REF_UI_INVENTORY_WEIGHT);
	if (totalWeight > 55) {
		ui.element[ui.getElementId(id)].textColor = sf::Color(255, 0, 0);
	}
	access.unlock();
}

void cUI::updateInterfaceEquipment()
{
	access.lock();
	ui.removeElementsByRef(REF_UI_ACTIVEITEM);

	float resModX = (float)camera.res.x / 1280.00f;
	float resModY = (float)camera.res.y / 800.00f;

	// Get equipped items
	cItemContainer cont = game.getUnit(client.unit).container;
	cItem equip[4];
	int itemsFound = 0, id = 0;
	for (int i = 0; i < cont.itemCounter; i++)
	{
		if (cont.item[i].equipped) {
			equip[itemsFound++] = cont.item[i];
			if (itemsFound >= 4) { i = cont.itemCounter; }
		}
	}
	
	// Hidden button
	int baseId = ui.addElement("button", vec2(-2000.00f, 0));
	ui.element[ui.getElementId(id)].size = sf::Vector2f(40, 40);
	ui.element[ui.getElementId(id)].texture = visual.addTexture("ui_icon_empty_white.png");
	ui.element[ui.getElementId(id)].textureHovered = visual.addTexture("ui_icon_empty_orange.png");
	ui.element[ui.getElementId(id)].button.action = "activeItem";
	ui.element[ui.getElementId(id)].button.args[0] = to_string(equip[0].globalId);
	ui.element[ui.getElementId(id)].addRef(REF_UI_ACTIVEITEM);
	ui.element[ui.getElementId(id)].tooltip.setText(equip[0].displayName);
	ui.element[ui.getElementId(id)].tooltip.offset.x = 12.00f;
	ui.element[ui.getElementId(id)].tooltip.offset.y = -6.00f;
	ui.element[ui.getElementId(id)].tooltip.delay = 0.50f;
	ui.element[ui.getElementId(id)].tooltip.ignoreOrigin = true;
	ui.element[ui.getElementId(id)].priority = 4;
	ui.element[ui.getElementId(id)].setText("1");
	ui.element[ui.getElementId(id)].textColorHover = sf::Color(255, 150, 0);
	ui.element[ui.getElementId(id)].textOffset = vec2(8.00f, 5.00f);
	ui.element[ui.getElementId(id)].textSize = 12;

	// Actual buttons
	for (int i = 0; i < itemsFound; i++)
	{
		id = ui.addElement(ui.element[ui.getElementId(baseId)], vec2(20.00f + 40.00f * floor(i / (LIMIT_BUTTONSPERCOLUMN + 1)),
						550.00f * resModY + 40.00f * (i % (LIMIT_BUTTONSPERCOLUMN + 1)) ) );

		ui.element[ui.getElementId(id)].addRef(REF_UI_ACTIVEITEM + i + 1);
		ui.element[ui.getElementId(id)].button.action = "activeItem";
		ui.element[ui.getElementId(id)].button.args[0] = to_string(equip[i].globalId);
		ui.element[ui.getElementId(id)].tooltip.setText(equip[i].displayName);
		ui.element[ui.getElementId(id)].setText(to_string(i + 1));
	}
	access.unlock();
}

void cUI::updateFull()
{
	access.lock();
	removeElementsByRef(-1);

	int id = ui.addElement("icon_health", sf::Vector2f(32, 32));
	ui.element[ui.getElementId(id)].addRef(REF_UI_HEALTHBAR);
	ui.element[ui.getElementId(id)].bar.minOffset = 0.20f;
	ui.element[ui.getElementId(id)].bar.maxOffset = 0.20f;
	ui.element[ui.getElementId(id)].bar.vertical = true;

	id = ui.addElement("button_test", sf::Vector2f(camera.res.x - 70.00f, 20.00f));
	ui.element[ui.getElementId(id)].button.action = "invToggle";
	ui.element[ui.getElementId(id)].setText("Inventory");
	ui.element[ui.getElementId(id)].tooltip.setText("Open inventory screen");
	ui.element[ui.getElementId(id)].addRef(REF_UI_INVENTORY_BUTTON);

	access.unlock();
}