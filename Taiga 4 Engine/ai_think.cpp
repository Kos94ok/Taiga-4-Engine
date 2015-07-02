
#include "main.h"
#include "ai.h"
#include "game.h"
#include "math.h"

void cAI::think_enemy(int globalId)
{
	cUnit* me = &game.getUnit(globalId);

	std::vector<int> idList;
	for (int i = 0; i < game.unitCounter; i++)
	{
		if (game.unit[i].type == "player")
		{
			idList.push_back(game.unit[i].globalId);
		}
	}
	if (idList.size() > 0)
	{
		int target = idList[math.rand(0, (int)idList.size() - 1)];

		if (math.getDistance(me, &game.getUnit(target)) < 50.00f)
		{
			game.getUnit(target).addHealth(-value.enemyDamage);
		}
		else { me->addOrder_moveto_path(game.getUnit(target).pos); }
	}
}