
#include "main.h"

void cWorld::genTaigaMini()
{
	string wargs[] = { "0", "0", "0", "0" };
	// Creating some trees
	for (int i = 0; i < math.rand(500, 600); i++)
	{
		wargs[0] = "tree_basic_a";	wargs[1] = std::to_string(math.rand(-camera.res.x * 2, camera.res.x * 2));
									wargs[2] = std::to_string(math.rand(-camera.res.y * 2, camera.res.y * 2));
		cmd_unit_add(wargs);
	}
	// Creating some units
	for (int i = 0; i < 10; i++)
	{
		int id = game.addUnit("campfire_full",
			sf::Vector2f(math.rand(-camera.res.x * 2, camera.res.x * 2), math.rand(-camera.res.y * 2, camera.res.y * 2)));
		id = game.getUnitId(id);
		if (id != -1) {
			game.unit[id].light.flickerCurTime = math.randf(0.00f, game.unit[game.getUnitId(id)].light.flickerTime);
		}
	}

	// Moving the camera
	camera.moveto(sf::Vector2f(0.00f, 0.00f));
}

void cWorld::genArena()
{
	database.texture[TEX_WORLD_GROUND] = visual.addTexture("bg_neon.jpg");

	float arenaSizeX = 2000.00f;
	float arenaSizeY = 1200.00f;
	// Left
	int id = game.addUnit("arena_wall", vec2(0.00f, 0.00f));
	game.getUnit(id).size.y = arenaSizeY;
	// Top
	id = game.addUnit("arena_wall", vec2(20.00f, 0.00f));
	game.getUnit(id).size.x = arenaSizeX - 20.00f;
	// Right
	id = game.addUnit("arena_wall", vec2(arenaSizeX, 0.00f));
	game.getUnit(id).size.y = arenaSizeY;
	// Bottom
	id = game.addUnit("arena_wall", vec2(0.00f, arenaSizeY));
	game.getUnit(id).size.x = arenaSizeX + 20.00f;
}