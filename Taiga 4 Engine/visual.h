
#include "define.h"

class cTexture
{
public:
	std::string name;
	sf::Texture handle;
};

class cVisual
{
public:
	int maxTextureSize;

	int gameTexCounter;
	cTexture gameTex[LIMIT_TEXTURE];

	sf::Font fontMain;
	sf::Font fontDescr;
	sf::Shader shader[LIMIT_SHADERS];

	float shadowBrightness;
	int unitsPainted;

	void init();
	int addTexture(std::string name, bool ignoreFilter = false);

	cVisual() {
		unitsPainted = 0;
		gameTexCounter = 0;
		maxTextureSize = sf::Texture::getMaximumSize();
	}
};