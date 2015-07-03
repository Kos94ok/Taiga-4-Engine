
#pragma once
#include "util.h"

class cArg
{
public:
	std::string data[4];

	cArg(std::string a = "0", std::string b = "0", std::string c = "0", std::string d = "0") {
		data[0] = a;
		data[1] = b;
		data[2] = c;
		data[3] = d;
	}

	cArg(int i) {
		cArg();
	}

	std::string operator [](int i) const { return data[i]; }
	std::string & operator [](int i) { return data[i]; }
};

class cScript
{
public:
	cMutex access;

	std::vector<bool> threadReady;
	std::vector<std::thread> threadVector;
	std::vector<std::function<void(cArg)>> functionQueue;
	std::vector<cArg> functionArgs;

	void execute(std::function<void(cArg)> scriptFunc, cArg args = 0);
	int getFreeThread();

	static void threadMain(int index);
	void wait(int time);

	cScript() 
	{
		for (int i = 0; i < LIMIT_SCRIPTCOUNT; i++)
		{
			threadReady.push_back(false);
			threadVector.push_back(std::thread(cScript::threadMain, i));
		}
	}

	// Script data
	static void shooter_spawnEnemies(cArg args);
	static void shooter_respawnSystem(cArg args);
	static void shooter_spawnItems(cArg args);
	static void test_unitAddSystem(cArg args);
	static void test_consoleSystem(cArg args);

	static void ui_initialMenu(cArg args);
	static void server_sendChunkData(cArg args);
	static void unit_bindToMouse(cArg args);
};

extern cScript script;