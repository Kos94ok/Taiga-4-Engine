
#include "main.h"

void cEditor::genBlueprint()
{
	float dist = 1000.00f, localDist;
	vec2 pos;
	game.clearUnits();
	for (int i = 0; i < math.rand(50, 120); i++)
	{
		dist = 1000.00f;
		pos = vec2(math.randf(-LIMIT_CHUNKSIZE, LIMIT_CHUNKSIZE), math.randf(-LIMIT_CHUNKSIZE, LIMIT_CHUNKSIZE));
		for (int y = 0; y < game.unitCounter; y++)
		{
			localDist = math.getDistance(game.unit[y].pos, pos);
			if (localDist < dist) { dist = localDist; }
		}

		if (dist > 75.00f) { game.addUnit("tree_basic_", pos, -1, math.rand(0, 3)); }
	}
}