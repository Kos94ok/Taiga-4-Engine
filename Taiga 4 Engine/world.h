
#include "define.h"

class cBlueprintAttach
{
public:
	float x;
	float y;
	int direction;

	cBlueprintAttach() {
		x = 0.00f;
		y = 0.00f;
		direction = DIRECTION_RIGHT;
	}
};

class cBlueprint
{
public:
	std::string name;
	std::vector<cBlueprintAttach> attach;
};

class cWorld
{
public:
	std::vector<cBlueprint> blueprint;

	void analyzeBlueprints();
	sf::Vector2f applyBlueprint(sf::Vector2f position, int index, cBlueprintAttach *attach);
	std::vector<cUnitEntry> getBlueprintUnitList(int index);

	void genTaigaMini();
	void genArena();
	void genNormalWorld();
};