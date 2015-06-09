
#include "main.h"

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
	int target = idList[math.rand(0, (int)idList.size() - 1)];

	me->addOrder_moveto_path(game.getUnit(target).pos);
}