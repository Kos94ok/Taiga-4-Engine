
#include "main.h"

void cWorld::genTaigaMini()
{
	string type;
	int random;
	// Creating some trees
	for (int i = 0; i < math.rand(500, 600); i++)
	{
		random = math.rand(0, 3);
		if (random == 0) { type = "tree_basic_a"; }
		else if (random == 1) { type = "tree_basic_b"; }
		else if (random == 2) { type = "tree_basic_c"; }
		else { type = "tree_basic_d"; }
		game.addUnit(type, vec2(math.rand(-camera.res.x * 2, camera.res.x * 2), math.rand(-camera.res.y * 2, camera.res.y * 2)));
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

void cWorld::genNormalWorld()
{
	// Clearing the saved world data;
	clearWorld();
	// Settings initial state
	for (int j = 0; j < LIMIT_MAP; j++)
	{
		for (int i = 0; i < LIMIT_MAP; i++)
		{
			map[i][j].genStatus = CHUNK_UNDEFINED;
		}
	}

	// Generating spawn point
	vec2i spawnPointI = vec2i(math.rand(7, 55), math.rand(7, 55));

	// Generating paths
	for (int i = 0; i < settings.wgStartingPath; i++)
	{
		genChunkPath(1, spawnPointI);
	}

	// Writing global spawn point
	spawnPoint = vec2(spawnPointI.x * LIMIT_CHUNKSIZE, spawnPointI.y * LIMIT_CHUNKSIZE);

	// Applying spawn point
	map[spawnPointI.x][spawnPointI.y].genStatus = CHUNK_SPAWN;

	// Creating a wall around the normal chunks
	for (int j = 1; j < LIMIT_MAP - 1; j++)
	{
		for (int i = 1; i < LIMIT_MAP - 1; i++)
		{
			if (map[i][j].genStatus == CHUNK_UNDEFINED &&
				(map[i + 1][j].genStatus == CHUNK_NORMAL || map[i + 1][j].genStatus == CHUNK_SPAWN
				|| map[i + 1][j + 1].genStatus == CHUNK_NORMAL || map[i + 1][j + 1].genStatus == CHUNK_SPAWN
				|| map[i][j + 1].genStatus == CHUNK_NORMAL || map[i][j + 1].genStatus == CHUNK_SPAWN
				|| map[i - 1][j + 1].genStatus == CHUNK_NORMAL || map[i - 1][j + 1].genStatus == CHUNK_SPAWN
				|| map[i - 1][j].genStatus == CHUNK_NORMAL || map[i - 1][j].genStatus == CHUNK_SPAWN
				|| map[i - 1][j - 1].genStatus == CHUNK_NORMAL || map[i - 1][j - 1].genStatus == CHUNK_SPAWN
				|| map[i][j - 1].genStatus == CHUNK_NORMAL || map[i][j - 1].genStatus == CHUNK_SPAWN
				|| map[i + 1][j - 1].genStatus == CHUNK_NORMAL || map[i + 1][j - 1].genStatus == CHUNK_SPAWN))
			{
				map[i][j].genStatus = CHUNK_BLOCKED;
			}
		}
	}

	// Blocking the edges
	for (int i = 0; i < LIMIT_MAP; i++)
	{
		map[i][0].genStatus = CHUNK_BLOCKED;
		map[i][63].genStatus = CHUNK_BLOCKED;
	}
	for (int j = 0; j < LIMIT_MAP; j++)
	{
		map[0][j].genStatus = CHUNK_BLOCKED;
		map[63][j].genStatus = CHUNK_BLOCKED;
	}

	ofstream file;
	file.open("map.txt");
	for (int j = 0; j < LIMIT_MAP; j++)
	{
		for (int i = 0; i < LIMIT_MAP; i++)
		{
			if (map[i][j].genStatus == CHUNK_UNDEFINED) { file << "? "; }
			else if (map[i][j].genStatus == CHUNK_NORMAL) { file << "~ "; }
			else if (map[i][j].genStatus == CHUNK_BLOCKED) { file << "X "; }
			else if (map[i][j].genStatus == CHUNK_SPAWN) { file << "@ "; }
			else if (map[i][j].genStatus == CHUNK_VILLAGE) { file << "& "; }
		}
		file << "\n";
	}
	file.close();

	// Creating directory
	string name = "Savefiles//" + save.worldName;
	CreateDirectoryA("Savefiles", 0);
	CreateDirectoryA(name.c_str(), 0);
	// Copying blueprints to real positions
	for (int j = 0; j < LIMIT_MAP; j++)
	{
		for (int i = 0; i < LIMIT_MAP; i++)
		{
			if (map[i][j].genStatus != CHUNK_UNDEFINED)
			//if (map[i][j].genStatus == CHUNK_NORMAL)
			{
				applyBlueprint(vec2i(i, j), map[i][j].genStatus);
			}
		}
	}
}

void cWorld::genChunkPath(int val, vec2i pos)
{
	vec2i newPos;
	int random = math.rand(0, 3), saver = 0;

	// Looking for a non-claimed path
	do
	{
		newPos = pos;
		random = math.rand(0, 3);
		if (random == 0) { newPos.x += 1; }
		else if (random == 1) { newPos.y += 1; }
		else if (random == 2) { newPos.x -= 1; }
		else if (random == 3) { newPos.y -= 1; }
		saver += 1;
	}
	while (saver < 10 && map[newPos.x][newPos.y].genStatus != CHUNK_UNDEFINED);

	// If the path has found
	if (saver < 10)
	{
		pos = newPos;
		map[pos.x][pos.y].genStatus = CHUNK_NORMAL;

		// Termination chance
		if (math.rand(settings.wgMinimalPathLength, settings.wgMaximalPathLength) < val) { return; }
			// Not terminated - continue
		genChunkPath(val + 1, pos);
			// Chance to fork
		if (math.rand(0, 100) < settings.wgPathForkChance) { genChunkPath(val + 1, pos); }
	}
	else { return; }
}