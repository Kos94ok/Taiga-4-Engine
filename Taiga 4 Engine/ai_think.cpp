
#include "main.h"
#include "ai.h"
#include "game.h"
#include "math.h"

void cAI::think_enemy(int globalId)
{
	cUnit* me = &game.getUnit(globalId);

	vector<int> idList;
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

void cAI::think_rabbit(int globalId)
{
	cUnit* me = &game.getUnit(globalId);

	// Determine new goal
	if (me->ai.int_goal == AIGOAL_NONE) {
		me->ai.int_goal = AIGOAL_WANDER;
	}

	// Act according to the goal
	if (me->ai.int_goal == AIGOAL_WANDER)
	{
		if (me->orderCounter == 0 && math.randf(0.00f, 1.00f) < 0.10f) {
			me->addOrder_moveto_path(me->pos + vec2f(-500.00f, 500.00f));
		}
	}
}