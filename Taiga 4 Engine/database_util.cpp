
#include "main.h"

cUnit& cDatabase::getUnit(string type)
{
	for (int i = 0; i < LIMIT_DB_UNIT; i++)
	{
		if (unit[i].type == type) { return unit[i]; }
	}
	// Return error-unit
	cout << "[ERROR] Can't find the unit [" << type << "] in the database!" << "\n";
	return unit[0];
}

cItem& cDatabase::getItem(string type)
{
	for (int i = 0; i < LIMIT_DB_ITEM; i++)
	{
		if (item[i].type == type) { return item[i]; }
	}
	// Return error-unit
	cout << "[ERROR] Can't find the item [" << type << "] in the database!" << "\n";
	return item[0];
}

cUIElement& cDatabase::getUIElement(string type)
{
	for (int i = 0; i < LIMIT_DB_UIELEMENTS; i++)
	{
		if (uiElement[i].type == type) { return uiElement[i]; }
	}
	// Return error-unit
	cout << "[ERROR] Can't find the UI element [" << type << "] in the database!" << "\n";
	return uiElement[0];
}

bool cDatabase::isItemGood(string type)
{
	for (int i = 0; i < LIMIT_DB_ITEM; i++)
	{
		if (item[i].type == type) { return true; }
	}
	return false;
}