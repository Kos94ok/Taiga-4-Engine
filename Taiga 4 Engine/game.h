
#pragma once
#include "precomp.h"
#include "unit.h"
#include "util.h"
#include "item.h"

class cGame
{
public:
	cMutex access;

	int unitCounter;
	cUnit unit[LIMIT_UNIT];

	int unitGlobalCounter;
	int itemGlobalCounter;

	float ambientLight;
	float ambientLightMin;
	float ambientLightMax;
	float timeOfDay;
	bool isDay();
	bool isNight();
	bool lightens();
	bool darkens();

	cGame() {
		ambientLight = 255.00f;
		ambientLightMin = 0.00f;
		ambientLightMax = 300.00f;
		timeOfDay = 12.00f;
		unitCounter = 0;
		unitGlobalCounter = 0;
		itemGlobalCounter = 0;
	}

	// Takes: Unit type, Unit position, Owner, Variation, Data send flag, Global Id
	// Returns: Unit global id
	int addUnit(std::string type, sf::Vector2f pos, int owner = -1, int variation = -1, bool sendData = true, int overrideGlobalId = -1);
	// Takes: Unit type, Unit position, Unit Global Id
	// Returns: Unit global id
	int addUnitID(std::string type, sf::Vector2f pos, int globalId = -1);
	// Takes: Unit global id
	// Returns: Nothing
	void removeUnit(int id, bool sendData = true);
	// Takes: Unit global id
	// Returns: Nothing
	void killUnit(int id);
	// Takes: Unit global id, Amount of damage
	// Returns: Nothing
	void damageUnit(int id, float damage);
	// Takes: Nothing
	// Returns: Nothing
	void clearUnits();
	// Takes: Unit global id
	// Returns: Unit local id
	int getUnitId(int globalId);
	// Takes: World location
	// Returns: First found unit's local id
	int getUnitId(sf::Vector2f pos, int refFilter = -1);
	// Takes: Unit global id
	// Returns: Pointer to unit
	cUnit& getUnit(int id);
	// Takes: World location
	// Returns: Pointer to first found unit (using interact distance)
	cUnit& getUnit(sf::Vector2f pos, int refFilter = -1);
	// Takes: World location
	// Returns: Pointer to first found unit (using physical distance)
	cUnit& getUnitPhys(sf::Vector2f &pos, int refFilter = -1);
	// Takes: Unit A, Unit B
	// Returns: Minimum required interact distance
	float getUnitInteractDistance(cUnit unitA, cUnit unitB);
	// Takes: Unit A, Unit B
	// Returns: Minimum required collision distance
	float getUnitCollisionDistance(cUnit *unitA, cUnit *unitB);
	// Takes: Unit A, Unit B
	// Returns: Closest point where interact is possible
	vec2f getUnitInteractPoint(cUnit unitA, cUnit unitB);
	// Takes: Unit type
	// Returns: Amount of units of that type
	int getUnitCount(std::string type);
	// Takes: Unit ref
	// Returns: Amount of units with that ref
	int getUnitCount(int refId);
	// Takes: Unit global id
	// Returns: Is unit alive?
	bool isUnitAlive(int globalId);
	// Takes: Unit global id
	// Returns: Is unit dead?
	bool isUnitDead(int globalId);
	// Takes: Position, Item list
	// Returns: Created unit global id
	int createDrop(sf::Vector2f pos, cItemDrop itemA, cItemDrop itemB = "", cItemDrop itemC = "", cItemDrop itemD = "", int unitId = -1);
	// Takes: Position, Item vector
	// Returns: Created unit global id
	int createDrop(sf::Vector2f pos, std::vector<cItemDrop> itemList, int unitId = -1);
	// Takes: Time (24-hour format)
	// Returns: Nothing
	void setTimeOfDay(float time);
	// Takes: Light level
	// Returns: Nothing
	void setAmbientLight(float light);

	// Takes: Item global id
	// Returns: Nothing
	void useItem(int id);
};

extern cGame game;