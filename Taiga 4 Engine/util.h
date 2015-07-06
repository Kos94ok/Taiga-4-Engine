
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
	std::string getCurrentTimeString();
	
	sf::Color convertUnitHighlightColor(int code);
	sf::Color parseOldschoolColorCode(std::string code);

	std::string buildRefToType(int ref);
	int typeToBuildRef(std::string str);
	
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