
#include "main.h"

typedef cComponent item;

void cCraft::addRecipe(cComponent result, int resBalance, cComponent ingrA,
				cComponent ingrB, cComponent ingrC, cComponent ingrD, cComponent ingrE,
				cComponent ingrF, cComponent ingrG, cComponent ingrH, cComponent ingrI)
{
	int ingrCount = 1;
	recipe[recipeCounter].result = result;
	recipe[recipeCounter].ingr[0] = ingrA;
	if (ingrB.type != "") { recipe[recipeCounter].ingr[ingrCount++] = ingrB; }
	if (ingrC.type != "") { recipe[recipeCounter].ingr[ingrCount++] = ingrC; }
	if (ingrD.type != "") { recipe[recipeCounter].ingr[ingrCount++] = ingrD; }
	if (ingrE.type != "") { recipe[recipeCounter].ingr[ingrCount++] = ingrE; }
	if (ingrF.type != "") { recipe[recipeCounter].ingr[ingrCount++] = ingrF; }
	if (ingrG.type != "") { recipe[recipeCounter].ingr[ingrCount++] = ingrG; }
	if (ingrH.type != "") { recipe[recipeCounter].ingr[ingrCount++] = ingrH; }
	recipe[recipeCounter].ingrCount = ingrCount;
	recipe[recipeCounter].resourceBalance = resBalance;
	recipeCounter += 1;
}

int cCraft::getActiveRecipeId()
{
	int id = 0, amount = 0;
	bool match = true;
	for (int i = 0; i < recipeCounter; i++)
	{
		if (cont.itemCounter == recipe[i].ingrCount)
		{
			for (int y = 0; y < recipe[i].ingrCount; y++)
			{
				id = cont.getId(recipe[i].ingr[y].type);
				amount = cont.getAmount(recipe[i].ingr[y].type);
				match = (id != -1 && amount >= recipe[i].ingr[y].count);
				if (!match) { y = recipe[i].ingrCount; }
			}
			if (match) { return i; }
		}
	}
	return -1;
}

int cCraft::getActiveRecipeRepeats()
{
	bool match = true;
	int i = getActiveRecipeId();
	int amount = 100;
	// Amount of ingridients
	for (int y = 0; y < recipe[i].ingrCount; y++)
	{
		amount = min(amount, craft.cont.getAmount(recipe[i].ingr[y].type) / recipe[i].ingr[y].count);
	}
	// Amount of resource
	if (recipe[i].resourceBalance < 0.00f)
	{
		amount = min(amount, math.round(game.getUnit(client.unit).resource / -recipe[i].resourceBalance));
	}
	return amount;
}

void cCraft::checkActiveRecipe()
{
	bool match = true;
	int matchId = -1;
	updateCraftAmounts();
	matchId = getActiveRecipeId();
	// If recipe found
	if (match)
	{
		int rpt = getActiveRecipeRepeats();
		resultCont.itemCounter = 0;
		resultCont.add(recipe[matchId].result.type, recipe[matchId].result.count * rpt);
	}
	// No recipe found
	else
	{
		resultCont.itemCounter = 0;
	}
}

void cCraft::getResult(bool craftAll)
{
	int rec = getActiveRecipeId();
	if (rec == -1 || getActiveRecipeRepeats() <= 0) { return; }
	int rpt = 1;
	if (craftAll) { rpt = getActiveRecipeRepeats(); }

	// Add crafted item
	//game.unit[game.getUnitId(client.unit)].container.add(craft.resultCont.item[0], craft.recipe[rec].result.count * rpt);

	// Remove stuff
	for (int i = 0; i < recipe[rec].ingrCount; i++)
	{
		cont.remove(recipe[rec].ingr[i].type, craft.recipe[rec].ingr[i].count * rpt);
	}
	// Temporary remove the resource
	game.unit[game.getUnitId(client.unit)].resource -= -craft.recipe[rec].resourceBalance * rpt;

	// Update amounts
	checkActiveRecipe();

	// Update UI
	//ui.updateInterfaceItemList();

	// Add ingridients back to inventory (will be removed by server command)
	game.unit[game.getUnitId(client.unit)].resource += -craft.recipe[rec].resourceBalance * rpt;
	for (int i = 0; i < recipe[rec].ingrCount; i++)
	{
		game.unit[game.getUnitId(client.unit)].container.add(recipe[rec].ingr[i].type, craft.recipe[rec].ingr[i].count * rpt);
	}

	// Data to server
	sf::Packet data;
	data << MSG_CONTROLS_CRAFTITEM << rec << rpt;
	client.sendPacket(data);
}

void cCraft::updateCraftAmounts()
{
	return;
	for (int i = 0; i < craft.cont.itemCounter; i++)
	{
		if (craft.cont.amount[i] > game.unit[game.getUnitId(client.unit)].container.getAmount(craft.cont.item[i].type))
		{
			craft.cont.amount[i] = game.unit[game.getUnitId(client.unit)].container.getAmount(craft.cont.item[i].type);
			if (craft.cont.amount[i] == 0) {
				craft.cont.remove(craft.cont.item[i].type);
				i -= 1;
			}
		}
	}
}

void cCraft::loadRecipes()
{
	addRecipe("voodoo_doll", -20, item("human_leg", 1), item("human_arm", 1), item("human_ear", 5));
	addRecipe("test_catAll", 0, "test_catWeapons", "test_catArmor", "test_catMaterials", "test_catConsumables", "test_catOther");
	addRecipe("test_resourceManipulator", 100, "test_resourceManipulator");
}