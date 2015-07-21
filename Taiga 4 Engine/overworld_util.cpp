
#include "main.h"
#include "overworld.h"

void cOverworld::linkMaps(int a, int b)
{
	cOverworldLink link;
	link.a = a;
	link.b = b;
	links.push_back(link);
}

bool cOverworld::isMapAvailable(int index)
{
	for (cOverworldLink link : links)
	{
		if (link.a == index && link.b == activeMap) { return true; }
		if (link.b == index && link.a == activeMap) { return true; }
	}
	return false;
}