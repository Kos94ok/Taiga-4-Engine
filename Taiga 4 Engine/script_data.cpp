
#include "main.h"

void cScript::shooter_spawnEnemies(cArg args)
{
	// Only for server
	if (!core.serverMode && !core.localServer) { return; }
	// Waiting for player spawn
	while (game.getUnitCount("player") == 0 && !core.shutdown) { Sleep(50); }
	// Spawning the enemies constantly
	while (!core.shutdown)
	{
		// Maximum enemy limit
		if (game.getUnitCount("enemy") < 5)
		{
			// Create a unit
			game.addUnit("enemy", world.spawnPoint + vec2f(math.randf(-500.00f, 500.00f), math.randf(-500.00f, 500.00f)));
		}
		// Wait
		script.wait(1000);
	}
}

void cScript::shooter_respawnSystem(cArg args)
{
	// Only for server
	if (!core.serverMode && !core.localServer) { return; }
	// Loop
	while (!core.shutdown)
	{
		script.wait(5000);
		for (int i = 0; i < LIMIT_SERVER_PLAYERS; i++)
		{
			if (server.player[i].connected && game.isUnitDead(server.player[i].unit))
			{
				server.assignUnit(i, game.addUnit("player", world.spawnPoint + vec2f(math.randf(-500.00f, 500.00f), math.randf(-500.00f, 500.00f))));
			}
		}
	}
}

void cScript::shooter_spawnItems(cArg args)
{
	// Only for server
	if (!core.serverMode && !core.localServer) { return; }
	// Spawning stuff
	game.createDrop(world.spawnPoint + vec2f(0.00f, -1000.00f), "human_eye");
	game.createDrop(world.spawnPoint + vec2f(0.00f, 1000.00f), "human_arm");
	game.createDrop(world.spawnPoint + vec2f(-1000.00f, 0.00f), "human_leg");
	game.createDrop(world.spawnPoint + vec2f(1000.00f, 0.00f), "human_ear");
	game.createDrop(world.spawnPoint + vec2f(-50.00f, 0.00f), "voodoo_recipe");
}

void cScript::test_unitAddSystem(cArg args)
{
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