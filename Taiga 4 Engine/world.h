
#include "define.h"

class cBlueprint
{
public:
	std::string name;
};

class cMapTile
{
public:
	int genStatus;

	cMapTile() {
		genStatus = CHUNK_UNDEFINED;
	}
};

class cWorld
{
public:
	std::vector<cBlueprint> blueprint;
	cMapTile map[LIMIT_MAP][LIMIT_MAP];

	void analyzeBlueprints();
	sf::Vector2f applyBlueprint(sf::Vector2f position, int index);
	std::vector<cUnitEntry> getBlueprintUnitList(int index);

	void genTaigaMini();
	void genArena();
	void genNormalWorld();
	void genChunkPath(int val, sf::Vector2i pos);
};