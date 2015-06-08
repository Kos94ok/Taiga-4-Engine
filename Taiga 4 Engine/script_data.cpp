
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

void cScript::test_unitAddSystem(cArg args)
{
	return;
	script.wait(5000);

	const int count = 100;

	int number[count];

	for (int a = 0; a < 100; a++)
	{
		for (int i = 0; i < count; i++)
		{
			number[i] = game.addUnit("missingno", vec2f(0.00f, 0.00f));
		}
		for (int i = 0; i < count; i++)
		{
			game.removeUnit(number[i]);
		}

		if (a % 2 == 0) { ui.openInventory(); }
		else { ui.closeInventory(); }
	}
}