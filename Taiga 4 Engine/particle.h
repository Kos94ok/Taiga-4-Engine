
#include "unit.h"

class cParticle
{
public:
	int owner;
	int globalId;
	std::string type;
	sf::Vector2f pos;
	sf::Vector2f center;
	sf::Vector2f size;
	float flyingHeight;

	cParticle() {
	}
};