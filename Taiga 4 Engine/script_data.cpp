
#include "main.h"

void cScript::spawnEnemies(cArg args)
{
	cout << "Starting some script..." << endl;
	for (int i = 0; i < 10; i++)
	{
		game.addUnit("enemy", game.getUnit(client.unit).pos + vec2f(50.00f, 50.00f));
		script.wait(1000);
	}
}