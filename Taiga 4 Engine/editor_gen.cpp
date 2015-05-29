
#include "main.h"

void cEditor::genBlueprint_Normal()
{
	float dist = 1000.00f, localDist;
	vec2 pos;
	game.clearUnits();
	int total = 0;
	// Trees
	for (int i = 0; i < math.rand(6, 12) && total < 100; i++)
	{
		dist = 1000.00f;
		pos = vec2(math.randf(-LIMIT_CHUNKSIZE / 2, LIMIT_CHUNKSIZE / 2), math.randf(-LIMIT_CHUNKSIZE / 2, LIMIT_CHUNKSIZE / 2));
		for (int y = 0; y < game.unitCounter; y++)
		{
			localDist = math.getDistance(game.unit[y].pos, pos);
			if (localDist < dist) { dist = localDist; }
		}

		if (dist > 125.00f) { game.addUnit("tree_basic_", pos, -1, math.rand(0, 3)); }
		else { i -= 1; }
		total += 1;
	}
	total = 0;

	// Stones
	for (int i = 0; i < math.rand(5, 15) && total < 100; i++)
	{
		pos = vec2(math.randf(-LIMIT_CHUNKSIZE / 2, LIMIT_CHUNKSIZE / 2), math.randf(-LIMIT_CHUNKSIZE / 2, LIMIT_CHUNKSIZE / 2));
		for (int y = 0; y < game.unitCounter; y++)
		{
			localDist = math.getDistance(game.unit[y].pos, pos);
			if (localDist < dist) { dist = localDist; }
		}

		if (dist > 125.00f) { game.addUnit("stone_basic_", pos, -1, math.rand(0, 3)); }
		else { i -= 1; }
		total += 1;
	}
}

void cEditor::genBlueprint_Blocked()
{
	vec2 pos;
	game.clearUnits();
	int total = 0;
	for (int i = 0; i < math.rand(25, 50) && total < 100; i++)
	{
		pos = vec2(math.randf(-LIMIT_CHUNKSIZE / 2, LIMIT_CHUNKSIZE / 2), math.randf(-LIMIT_CHUNKSIZE / 2, LIMIT_CHUNKSIZE / 2));
		game.addUnit("tree_basic_", pos, -1, math.rand(0, 3));
		total += 1;
	}
}

void cEditor::genBlueprint()
{
	if (autogenType == CHUNK_NORMAL) { genBlueprint_Normal(); }
	else if (autogenType == CHUNK_BLOCKED) { genBlueprint_Blocked(); }
	else { cout << "[cEditor::genBlueprint] Unknown autogenType (" << autogenType << ")\n"; }
}