
#pragma once
#include "precomp.h"
#include "util.h"
#include "saveload.h"

#define CHUNK_UNDEFINED					0
#define CHUNK_NORMAL					1
#define CHUNK_BLOCKED					2
#define CHUNK_SPAWN						3
#define CHUNK_VILLAGE					4
#define CHUNK_LOOT_COMMON				5
#define CHUNK_LOOT_GOOD					6
#define CHUNK_LOOT_RARE					7
#define CHUNK_LOOT_PERFECT				8
#define CHUNK_TEMP						100

void worldLoaderMain();

class cBlueprint
{
	/*
	cBlueprint is used to store the blueprint data in the memory.
	*/
public:
	int type;
	string name;
};

class cMapTile
{
public:
	int blueprint;
	bool isLoaded;
	bool isInitialized;
	int type;

	cMapTile() {
		blueprint = -1;
		isLoaded = false;
		isInitialized = false;
		type = CHUNK_UNDEFINED;
	}
};

class cWorld
{
public:
	cMutex access;

	vec2f spawnPoint;
	vector<cBlueprint> blueprint;
	cMapTile map[LIMIT_MAP][LIMIT_MAP];

	void analyzeBlueprints();
	void applyBlueprint(vec2i position, int type);
	vector<cUnitEntry> getBlueprintUnitList(int index);
	vector<cUnitEntry> getChunkUnitList(vec2i pos);
	vector<int> unloadVector;

	void genTaigaMini();
	void genArena();
	void genNormalWorld();
	void genChunkPath(int val, vec2i pos);
	void clearWorld();

	bool isChunkLoaded(vec2i pos);
	bool isChunkViable(vec2i pos);
	void saveChunk(vec2i pos);
	void loadChunk(vec2i pos);
	void unloadChunk(vec2i pos);
	void saveAll();
	void unloadAll();
	void applyUnload();
	sf::Vector2i getChunkInPos(vec2f pos);
	sf::Vector2f getChunkCenter(vec2i pos);
};

extern cWorld world;