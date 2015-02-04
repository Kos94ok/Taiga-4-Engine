
#include "main.h"

void cVisual::init()
{
	// Initializing the fonts
	// Main font
	if (!fontMain.loadFromFile("Data/Fonts/stylo.ttf"))
	{
		if (!fontMain.loadFromFile("C:/Windows/Fonts/constan.ttf"))
		{
			fontMain.loadFromFile("C:/Windows/Fonts/times.ttf");
		}
	}
	// Descr font
	if (!fontDescr.loadFromFile("C:/Windows/Fonts/constan.ttf"))
	{
		fontDescr.loadFromFile("C:/Windows/Fonts/times.ttf");
	}
	// Loading the shaders
	shader[SHADER_FULLSCREEN].loadFromFile("Data/Shaders/universal.frag", sf::Shader::Fragment);
	shader[SHADER_BLOOM].loadFromFile("Data/Shaders/bloom.frag", sf::Shader::Fragment);
	shader[SHADER_LIGHT].loadFromFile("Data/Shaders/light.frag", sf::Shader::Fragment);
	shader[SHADER_SHADOW].loadFromFile("Data/Shaders/shadow.frag", sf::Shader::Fragment);
	shader[SHADER_CAMBLUR].loadFromFile("Data/Shaders/camblur.frag", sf::Shader::Fragment);
}

int cVisual::addTexture(string name, bool ignoreFilter)
{
	// Looking for the texture
	for (int i = 0; i < gameTexCounter; i++)
	{
		if (visual.gameTex[i].name == name) { return i; }
	}
	// Adding the texture
	string filepath = "Data\\Textures\\" + name;
	visual.gameTex[visual.gameTexCounter].name = name;
	visual.gameTex[visual.gameTexCounter].handle.setRepeated(true);
	if (!ignoreFilter) { visual.gameTex[visual.gameTexCounter].handle.setSmooth(math.intToBool(settings.enableTextureSmoothing)); }
	visual.gameTex[visual.gameTexCounter].handle.loadFromFile(filepath);
	visual.gameTexCounter += 1;
	return visual.gameTexCounter - 1;
}