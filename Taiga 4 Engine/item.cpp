
#include "main.h"

void cItem::setName(string txt)
{
	displayName = txt;
}

void cItem::addDescrLine(string line)
{
	description[descrLinesUsed++] = line;
}

int cItemContainer::add(cItem target, int count)
{
	int search = getId(target.type);
	if (search != -1)
	{
		amount[search] += count;
		return item[search].globalId;
	}
	else if (itemCounter < LIMIT_ITEMS && itemCounter < itemLimit)
	{
		item[itemCounter] = target;
		item[itemCounter].globalId = game.itemGlobalCounter++;
		amount[itemCounter] = count;
		itemCounter += 1;
		return game.itemGlobalCounter - 1;
	}
	return -1;
}

int cItemContainer::add(std::string type, int count)
{
	return add(database.getItem(type), count);
}

cItem& cItemContainer::get(int id)
{
	id = getId(id);
	if (id != -1) { return item[id]; }
	return database.item[0];
}

cItem& cItemContainer::get(std::string type, int begin)
{
	for (int i = begin; i < itemCounter; i++)
	{
		if (item[i].type == type) { return item[i]; }
	}
	return database.item[0];
}

bool cItemContainer::remove(int id, int count)
{
	id = getId(id);
	if (id != -1)
	{
		if (count == -1 || amount[id] <= count)
		{
			for (int i = id; i < itemCounter - 1; i++)
			{
				item[i] = item[i + 1];
				amount[i] = amount[i + 1];
			}
			itemCounter -= 1;
		}
		else { amount[id] -= count; }
		return true;
	}
	return false;
}

bool cItemContainer::remove(string type, int count, int begin)
{
	for (int i = begin; i < itemCounter; i++)
	{
		if (item[i].type == type)
		{
			remove(item[i].globalId, count);
			return true;
		}
	}
	return false;
}

int cItemContainer::getId(int id)
{
	for (int i = 0; i < itemCounter; i++)
	{
		if (item[i].globalId == id) { return i; }
	}
	return -1;
}

int cItemContainer::getId(string id, int begin)
{
	for (int i = begin; i < itemCounter; i++)
	{
		if (item[i].type == id) { return i; }
	}
	return -1;
}

int cItemContainer::getAmount(string type)
{
	int id = getId(type);
	if (id == -1) { return 0; }
	return amount[id];
}