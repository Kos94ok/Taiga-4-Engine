
#pragma once
#include "precomp.h"

class cPacketQueue
{
public:
	int id;
	int target;
	sf::Packet data;
};

class cColor
{
public:
	int r;
	int g;
	int b;
	int a;

	cColor() {
		r = 0;
		g = 0;
		b = 0;
		a = 255;
	};

	cColor(int R, int G, int B, int A) {
		r = R;
		g = G;
		b = B;
		a = A;
	}
};

class cCodeStats
{
public:
	int linesOfCode;
	int headerFiles;
	int sourceFiles;

	void calculate();
};

extern cCodeStats codeStats;

class cUtil
{
public:
	void detectVideoCard();
	int getInventorySortingType();
	void checkLog(string filename);
	void checkLogFiles();
	string getCurrentTimeString(bool removeSemi = false, bool includeDay = false);
	
	sf::Color convertUnitHighlightColor(int code);
	sf::Color parseOldschoolColorCode(string code);
	string buildRefToType(int ref);
	int typeToBuildRef(string str);
	string buffIdToType(int buff);
	int typeToBuffId(string str);

	bool screenshotRequested;
	void makeScreenshot();

	void checkLaunchStatus();
	
	bool intersects(sf::Vector2f pos, sf::Vector2f rectPos, sf::Vector2f rectSize);
};

extern cUtil util;

class cValue
{
public:
	const float rifleDamage = 10.00f;
	const float rifleMaxDistance = 1750.00f;

	const float maxBuildDist = 200.00f;

	const float enemyDamage = 10.00f;

	const float chatNoFocusTimer = 3.50f;
	const float chatRecentMessageTimer = 1.50f;
	const float progressBarFadeInTimer = 0.10f;
	const float progressBarFadeOutTimer = 0.30f;

	const float statCold_normal_day = 1.00f;
	const float statCold_normal_night = 1.30f;
	const float statHunger_normal = 0.30f;

	float getMinColdLocal() { return -5.00f; }
	float getMaxColdLocal() { return 100.00f; }
	float getMinHungerLocal() { return -5.00f; }
	float getMaxHungerLocal() { return 100.00f; }
};

extern cValue value;

class cMutex
{
public:
	sf::Mutex mutex;

	void lock() {
		mutex.lock();
	}
	void unlock() {
		mutex.unlock();
	}
};

class cMutexGlobal
{
public:
	cMutex animation;
	cMutex renderMain;
	cMutex renderUnits;
	cMutex renderClouds;
	cMutex renderParticles;
	cMutex mainMatrix;

	cMutex serverHistory;
	cMutex serverPacketQueue;
};

extern cMutexGlobal mutex;

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