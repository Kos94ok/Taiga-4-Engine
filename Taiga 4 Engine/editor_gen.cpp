
#include "main.h"

void cEditor::genBlueprint()
{
	float dist = 1000.00f, localDist;
	vec2 pos;
	game.clearUnits();
	int total = 0;
	for (int i = 0; i < math.rand(8, 15) && total < 100; i++)
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
}