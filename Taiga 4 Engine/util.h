
#pragma once
#include "precomp.h"

class cPacketQueue
{
public:
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

class cUtil
{
public:
	void detectVideoCard();
	int getInventorySortingType();
	void checkLog(std::string filename);
	void checkLogFiles();
	std::string getCurrentTimeString(bool removeSemi = false, bool includeDay = false);
	
	sf::Color convertUnitHighlightColor(int code);
	sf::Color parseOldschoolColorCode(std::string code);

	std::string buildRefToType(int ref);
	int typeToBuildRef(std::string str);

	bool screenshotRequested;
	void makeScreenshot();
	
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

	const float chatNoFocusTimer = 7.50f;
	const float chatRecentMessageTimer = 7.00f;
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
	cMutex mainMatrix;
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