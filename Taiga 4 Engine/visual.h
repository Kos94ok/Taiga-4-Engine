
#pragma once
#include "precomp.h"
#include "define.h"

class cTexture
{
public:
	std::string name;
	sf::Texture handle;
};

class cMousePointer
{
public:
	int type;
};

class cProgressBar
{
public:
	bool enabled;
	bool manualMode;
	float manualModeStep;
	float timeCurrent;
	float timeMaximum;
	float fadeTimer;
	int getTexFrameCountX();
	int getTexFrameCountY();
	float getCurrentAlpha();
	sf::Vector2i getCurrentFrame();
	sf::Vector2i getTexFrameSize();
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
	cMousePointer mouse;
	cProgressBar progress;

	float shadowBrightness;
	int unitsPainted;
	int hoveredUnit;

	void init();
	int addTexture(std::string name, bool ignoreFilter = false);
	int createIcon(std::string filename, std::string iconName, vec2i pos, vec2f gridSize);

	int genHighlighted(std::string filename);

	void setMousePointer(int tex);
	void enableProgressBar(float time);
	void enableProgressBarManual(float time, int stepCount);
	void advanceProgressBar();
	void disableProgressBar();

	cVisual() {
		hoveredUnit = -1;
		unitsPainted = 0;
		gameTexCounter = 0;
		maxTextureSize = sf::Texture::getMaximumSize();
	}
};

extern cVisual visual;