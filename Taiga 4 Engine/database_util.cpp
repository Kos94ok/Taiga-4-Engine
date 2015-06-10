
#include "main.h"

cUnit& cDatabase::getUnit(string type)
{
	access.lock();
	for (int i = 0; i < LIMIT_DB_UNIT; i++)
	{
		if (unit[i].type == type) { access.unlock(); return unit[i]; }
	}
	// Return error-unit
	console << "[ERROR] Can't find the unit [" << type << "] in the database!" << "\n";
	access.unlock();
	return unit[0];
}

cItem& cDatabase::getItem(string type)
{
	access.lock();
	for (int i = 0; i < LIMIT_DB_ITEM; i++)
	{
		if (item[i].type == type) { access.unlock(); return item[i]; }
	}
	// Return error-unit
	console << "[ERROR] Can't find the item [" << type << "] in the database!" << "\n";
	access.unlock();
	return item[0];
}

cUIElement& cDatabase::getUIElement(string type)
{
	access.lock();
	for (int i = 0; i < LIMIT_DB_UIELEMENTS; i++)
	{
		if (uiElement[i].type == type) { access.unlock(); return uiElement[i]; }
	}
	// Return error-unit
	console << "[ERROR] Can't find the UI element [" << type << "] in the database!" << "\n";
	access.unlock();
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