
#include "define.h"

class cGame
{
public:
	sf::Mutex access;

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

	// Takes: Unit type, Unit position
	// Returns: Unit global id
	int addUnit(std::string type, sf::Vector2f pos, int owner = -1, int variation = -1, bool sendData = true);
	// Takes: Unit global id
	// Returns: Nothing
	void removeUnit(int id, bool sendData = true);
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

	// Takes: Item global id
	// Returns: Nothing
	void useItem(int id);
};