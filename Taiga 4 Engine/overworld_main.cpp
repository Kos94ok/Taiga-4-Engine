
#include "main.h"
#include "overworld.h"
#include "math.h"

cOverworldMap newOverworldMap;
void cOverworld::generateMap()
{
	// Test maps
	newOverworldMap.id = mapCounter++;
	newOverworldMap.name = "Spawn";
	newOverworldMap.pos = vec2f(0.00f, 0.00f);
	newOverworldMap.level = 1;
	map.push_back(newOverworldMap);

	generatePath(0, vec2f(50.00f, 300.00f));

	/*entry.name = "Forest";
	entry.id = mapCounter++;
	entry.pos = vec2f(125.00f, 250.00f);
	entry.level = 1;
	map.push_back(entry);

	entry.name = "Forest";
	entry.id = mapCounter++;
	entry.pos = vec2f(100.00f, 200.00f);
	entry.level = 2;
	map.push_back(entry);

	// Test links
	linkMaps(0, 1);
	linkMaps(0, 2);*/

	// Cleaning up
	for (int i = 0; i < (int)map.size(); i++)
	{
		for (int y = 0; y < (int)map.size(); y++)
		{
			float dist = math.getDistance(map[i].pos, map[y].pos);
			if (i != y && dist < 75.00f)
			{
				map.erase(map.begin() + y);
				if (i >= y) { i -= 1; }
				y -= 1;
			}
		}
	}

	activeMap = 0;
}

void cOverworld::generatePath(int depth, vec2f pos, float angle)
{
	if (depth >= 5 && math.randf(0.00f, 1.00f) < 0.50f) { return; }

	if (angle >= 360.00f) { angle = math.randf(-90.00f, 0.00f); }

	pos = math.polar(pos, math.randf(100.00f, 200.00f), angle);
	pos.x = math.round(pos.x);
	pos.y = math.round(pos.y);

	newOverworldMap.name = "Unnamed";
	newOverworldMap.id = mapCounter++;
	newOverworldMap.pos = pos;
	newOverworldMap.level = 1;
	map.push_back(newOverworldMap);

	// Chance to fork
	if (math.randf(0.00f, 1.00f) < 0.70f) {
		float forkAngle = angle + math.randf(-90.00f, 90.00f);
		generatePath(depth + 1, pos, forkAngle);
	}

	// Continue current path
	generatePath(depth + 1, pos);
}