
#pragma once
#include "precomp.h"

class cOverworldMap
{
public:
	int id;

	vec2f pos;
	string name;
	string descr;
	int level;
};

class cOverworldLink
{
public:
	int a;
	int b;
};

class cOverworld
{
public:
	int mapCounter;
	int activeMap;
	vector<cOverworldMap> map;
	vector<cOverworldLink> links;

	void generateMap();
	void generatePath(int depth, vec2f pos, float angle = 720.00f);
	void linkMaps(int a, int b);
	bool isMapAvailable(int index);
	//void save(string saveName);
	//void load(string saveName);
};

extern cOverworld overworld;