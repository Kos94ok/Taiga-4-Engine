
#include "precomp.h"

class cGameLogic
{
public:
	void updateOrders(int elapsedTime);
	void updateAnim(int elapsedTime);
	void updateUI(int elapsedTime);
	void updateUnits(int elapsedTime);
	void updateConnection(int elapsedTime);
	void updateParticles(int elapsedTime);
	void updateSurvivalStats(int elapsedTime);

	void updateStatsFor(int unit, int player, float timevar);
};

extern cGameLogic gamelogic;
void gameLogicMain();
void animationMain();