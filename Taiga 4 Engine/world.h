
#include "define.h"

void worldLoaderMain();

class cBlueprint
{
public:
	int type;
	std::string name;
};

class cMapTile
{
public:
	int blueprint;
	bool isLoaded;
	int genStatus;

	cMapTile() {
		blueprint = -1;
		isLoaded = false;
		genStatus = CHUNK_UNDEFINED;
	}
};

class cWorld
{
public:
	sf::Mutex access;

	sf::Vector2f spawnPoint;
	std::vector<cBlueprint> blueprint;
	cMapTile map[LIMIT_MAP][LIMIT_MAP];

	void analyzeBlueprints();
	void applyBlueprint(sf::Vector2i position, int type);
	std::vector<cUnitEntry> getBlueprintUnitList(int index);
	std::vector<cUnitEntry> getChunkUnitList(sf::Vector2i pos);

	void genTaigaMini();
	void genArena();
	void genNormalWorld();
	void genChunkPath(int val, sf::Vector2i pos);
	void clearWorld();

	bool isChunkLoaded(sf::Vector2i pos);
	bool isChunkViable(sf::Vector2i pos);
	void saveChunk(sf::Vector2i pos);
	void loadChunk(sf::Vector2i pos);
	void unloadChunk(sf::Vector2i pos);
	sf::Vector2i getChunkInPos(sf::Vector2f pos);
	sf::Vector2f getChunkCenter(sf::Vector2i pos);
};