
#pragma once
#include "precomp.h"

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
	sf::Font fontConsole;
	sf::Shader shader[LIMIT_SHADERS];

	float shadowBrightness;
	int unitsPainted;
	int hoveredUnit;

	void init();
	int addTexture(std::string name, bool ignoreFilter = false);
	int createIcon(std::string filename, std::string iconName, vec2i pos, vec2f gridSize);

	int genHighlighted(std::string filename);

	cVisual() {
		hoveredUnit = -1;
		unitsPainted = 0;
		gameTexCounter = 0;
		maxTextureSize = sf::Texture::getMaximumSize();
	}
};

extern cVisual visual;