
#include "define.h"

class cUnitEntry
{
public:
	int globalId;
	std::string type;
	sf::Vector2f pos;

	cUnitEntry() {
		globalId = -1;
	}
};

class cSave
{
public:
	std::string worldName;

	std::string getChunkFilePath(sf::Vector2i pos);
	std::vector<cUnitEntry> getListFromFile(std::string filename);
	void flushListToFile(std::vector<cUnitEntry> list, std::string filename);

	cSave() {
		worldName = "world";
	}
};