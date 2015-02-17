
#include "define.h"

class cUnitEntry
{
public:
	std::string type;
	sf::Vector2f pos;
};

class cSave
{
public:
	std::string worldName;

	cSave() {
		worldName = "world";
	}
};