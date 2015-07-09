
#include "precomp.h"

class cGameLogic
{
public:
	void updateOrders(int elapsedTime);
	void updateAnim(int elapsedTime);
	void updateUI(int elapsedTime);
	void updateUnits(int elapsedTime);
	void updateConnection(int elapsedTime);
};

extern cGameLogic gamelogic;
void gameLogicMain();