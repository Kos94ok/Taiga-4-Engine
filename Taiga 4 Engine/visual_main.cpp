
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
	if (sf::Shader::isAvailable())
	{
		int shaderFail = 0;
		// Universal shader
		if (!shader[SHADER_FULLSCREEN].loadFromFile("Data/Shaders/universal.frag", sf::Shader::Fragment)) {
			window.showWarning(MSG_SHADERCOMPILE_UNIVERSAL);
			settings.enableScreenShaders = 0;
			settings.enableBetterShadows = 0;
			// Alternate light shader
			if (!shader[SHADER_LIGHT].loadFromFile("Data/Shaders/light.frag", sf::Shader::Fragment)) {
				window.showWarning(MSG_SHADERCOMPILE_LIGHT);
				settings.enableDynamicLight = 0;
				game.ambientLightMin = 10.00f;
			}
		}
		// Other shaders
		if (!shader[SHADER_BLOOM].loadFromFile("Data/Shaders/bloom.frag", sf::Shader::Fragment)) { shaderFail += 1; }
		if (!shader[SHADER_SHADOW].loadFromFile("Data/Shaders/shadow.frag", sf::Shader::Fragment)) { shaderFail += 1; }
		if (!shader[SHADER_CAMBLUR].loadFromFile("Data/Shaders/camblur.frag", sf::Shader::Fragment)) { shaderFail += 1; }
		if (shaderFail > 0) { window.showWarning(MSG_SHADERCOMPILE_OTHER); }
	}
	else { window.showWarning(MSG_NOSHADERSUPPORT); }
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