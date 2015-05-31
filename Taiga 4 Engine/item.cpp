
#include "main.h"

void cItem::setName(string txt)
{
	displayName = txt;
}

void cItem::addDescrLine(string line)
{
	description[descrLinesUsed++] = line;
	//description.push_back(line);
	//descrLinesUsed += 1;
}

int cItemContainer::add(cItem target, int count)
{
	int search = getId(target.type);
	if (search != -1)
	{
		amount[search] += count;
		sort(SORT_BYNAME);
		return item[search].globalId;
	}
	else if (itemCounter < LIMIT_ITEMS && itemCounter < itemLimit)
	{
		item[itemCounter] = target;
		item[itemCounter].globalId = game.itemGlobalCounter++;
		amount[itemCounter] = count;
		itemCounter += 1;
		sort(SORT_BYNAME);
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

cItem ex_item[LIMIT_ITEMS];
int ex_amount[LIMIT_ITEMS];
bool ex_ignore[LIMIT_ITEMS];
void cItemContainer::sort(int sortType)
{
	game.access.lock();
	for (int i = 0; i < LIMIT_ITEMS; i++) { ex_ignore[i] = false; }
	int itemsFound = 0, currentBestItem = -1;
	int charValueA = 0, charValueB = 0, charValueC = 0;
	int minCharValueA = 0, minCharValueB = 0, minCharValueC = 0;
	do
	{
		currentBestItem = -1;
		minCharValueA = 9999, minCharValueB = 9999, minCharValueC = 9999;
		for (int i = 0; i < itemCounter; i++)
		{
			if (!ex_ignore[i])
			{
				if (sortType == SORT_BYNAME)
				{
					charValueA = (int)item[i].displayName.c_str()[0];
					charValueB = (int)item[i].displayName.c_str()[1];
					charValueC = (int)item[i].displayName.c_str()[2];
					if (charValueA < minCharValueA) {
						minCharValueA = charValueA;
						currentBestItem = i;
					}
					else if (charValueA == minCharValueA && charValueB < minCharValueB) {
						minCharValueB = charValueB;
						currentBestItem = i;
					}
					else if (charValueA == minCharValueA && charValueB == minCharValueB && charValueC < minCharValueC) {
						minCharValueC = charValueC;
						currentBestItem = i;
					}
				}
			}
		}
		if (currentBestItem != -1) {
			ex_item[itemsFound] = item[currentBestItem];
			ex_amount[itemsFound] = amount[currentBestItem];
			ex_ignore[currentBestItem] = true;
			itemsFound += 1;
		}
	} while (currentBestItem != -1);

	for (int i = 0; i < itemCounter; i++)
	{
		item[i] = ex_item[i];
		amount[i] = ex_amount[i];
	}
	game.access.unlock();
}