
#include "main.h"

void cScript::spawnEnemies(cArg args)
{
	for (int i = 0; i < 10; i++)
	{
		while (client.unit == -1 && !core.shutdown) { Sleep(50); }
		game.addUnit("enemy", game.getUnit(client.unit).pos + vec2f(50.00f, 50.00f));
		script.wait(1000);
	}
}