
#include "buff.h"

void cBuffList::add(int type, float duration, int power)
{
	cBuff entry;
	entry.type = type;
	entry.duration = duration;
	entry.power = power;

	list.push_back(entry);
}

void cBuffList::remove(int type)
{
	for (int i = 0; i < (int)list.size(); i++) {
		if (list[i].type == type) {
			list.erase(list.begin() + i);
		}
	}
}

int cBuffList::getPower(int type)
{
	int retVal = 0;

	for (int i = 0; i < (int)list.size(); i++) {
		if (list[i].type == type) {
			retVal += list[i].power;
		}
	}

	return retVal;
}

float cBuffList::getDuration(int type)
{
	float retVal = 0.00f;

	for (int i = 0; i < (int)list.size(); i++) {
		if (list[i].type == type) {
			retVal += list[i].duration;
		}
	}

	return retVal;
}