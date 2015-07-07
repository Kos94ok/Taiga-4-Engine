
#include "main.h"
#include "settings.h"
#include "camera.h"
#include "window.h"
#include "math.h"

void cSettings::setDefault()
{
	// 0 - Low
	// 1 - Medium
	// 2 - High
	// 3 - Ultra
	this->unitDepthCheck = 1;				// Fixed value
	this->unitRenderDistance = 1;			// Fixed value

	// 0 - Low / Disabled
	// 1 - High / Enabled
	this->enableScreenShaders = 1;
	this->enableNightShadows = 1;
	this->enableBetterShadows = 1;
	this->enableDynamicLight = 1;			// Only disabled if shaders are not supported
	this->enableBetterLight = 1;
	this->enableTextureSmoothing = 0;
	this->enableCameraBlur = 1;
	this->enableVertSync = 1;
	this->enableQuickCast = 0; 
	this->enableLazyCast = 0;
	this->enableMouseScroll = 1;
	this->enableDynamicTooltips = 0;

	// Any value
	this->screenMode = 1;					// 0 - Window, 1 - Borderless, 2 - Fullscreen
	this->shadowBlur = 1;					// 1 - Disabled, 2+ - Shadow sample count	[Incompatible with better shadows]
	this->antialiasingLevel = 0;			// 0 - Disabled, 1+ - AA level
	this->sampleMod = 1.00f;				// 0.50 - Subsampling, 1.00 - Default, 2.00 - Supersampling

	// World generation
	this->wgStartingPath = 3;				// Amount of paths
	this->wgMinimalPathLength = 15;			// Minimal amount of steps before termination
	this->wgMaximalPathLength = 100;		// Maximum amount of steps before termination
	this->wgPathForkChance = 20;			// Chance to create another path on every step (percentage)
	/*
	Taiga Mini:
	wgStartingPath = 1;
	wgMinimalPathLength = 3;
	wgMaximalPathLength = 10;
	wgPathForkChance = 20;
	Taiga Maxi:
	wgStartingPath = 3;
	wgMinimalPathLength = 15;
	wgMaximalPathLength = 100;
	wgPathForkChance = 20;
	*/

	// General world settings
	this->wdDayNightSpeed = 1.00f / 72.00f;
	this->wdNightChangeSpeed = 1.00f;
	/*
	Arcade mode (fast):
	wdDayNightSpeed = 1.00f / 24.00f;
	wdNightChangeSpeed = 3.00f;
	Normal mode:
	wdDayNightSpeed = 1.00f / 72.00f;
	wdNightChangeSpeed = 1.00f;
	*/

	// Chat settings
	this->chatPosX = 0;
	this->chatPosY = -50;
	this->chatSizeX = 400;
	this->chatSizeY = 250;
	this->chatFontSize = 16;
	this->chatLineSpacing = 1;
	this->chatScrollSpeed = 1;
	this->enableChatTimestamps = 0;

	// Console settings
	this->enableConsoleTimestamps = 0;
	this->consoleScrollSpeed = 3;
	this->consoleFontSize = 14;
	this->consoleLineSpacing = 0;

	// Audio settings
	this->enableCameraListener = 1;
	this->volMaster = 1.00f;
	this->volMusic = 1.00f;
	this->volEffects = 1.00f;
	this->volAmbient = 1.00f;
	this->volWeather = 1.00f;
	this->volFootsteps = 1.00f;

	// Hotkeys
	this->hkInventory = sf::Keyboard::E;
		// Buttons [1; LIMIT) reserved for the items / abilities
	for (int i = 1; i < 10; i++)
	{
		this->hkActiveItem[i] = sf::Keyboard::Key(sf::Keyboard::Num0 + i);
	}
		// Up, Down, Left, Right
	this->hkCamMove[0] = sf::Keyboard::W;
	this->hkCamMove[1] = sf::Keyboard::S;
	this->hkCamMove[2] = sf::Keyboard::A;
	this->hkCamMove[3] = sf::Keyboard::D;
	this->hkCamToHero = sf::Keyboard::Space;
	this->hkChat = sf::Keyboard::Return;
	this->hkConsole = sf::Keyboard::Tilde;
	this->hkDebugMode = sf::Keyboard::F3;
	this->hkDebugAdvanced = sf::Keyboard::F4;
	this->hkScreenshot = sf::Keyboard::F12;

	// Garbage
	framerateLimit = 0;
	camera.res.x = window.getScreenSize().x;
	camera.res.y = window.getScreenSize().y;
	visualUnitHoverColor = sf::Color(255, 200, 50);
}

void cSettings::setUltra()
{
	this->unitDepthCheck = 3;
	this->unitRenderDistance = 3;

	this->enableScreenShaders = 1;
	this->enableNightShadows = 1;
	this->enableBetterShadows = 1;
	this->enableTextureSmoothing = 1;

	// Any value
	this->screenMode = 0;
	this->shadowBlur = 5;
	this->antialiasingLevel = 16;
	this->sampleMod = 2.00f;
}

void cSettings::load()
{
	bool retVal = true;
	cSettingsKey key;
	ifstream file;
	file.open("settings.ini");
	if (!file.good()) { console.error << "[ERROR] Can't open the settings file. Returning to default" << "\n"; return; }
	
	// Looking for values until the end of the file
	while (retVal)
	{
		retVal = getNextKey(&file, &key);
		// Matching the values
			// Display
		if (key.name == "screenMode") { screenMode = math.stringToInt(key.value); }
		else if (key.name == "screenWidth" && !core.serverMode) { camera.res.x = math.stringToInt(key.value); }
		else if (key.name == "screenHeight" && !core.serverMode) { camera.res.y = math.stringToInt(key.value); }
		else if (key.name == "consoleWidth" && core.serverMode) { camera.res.x = math.stringToInt(key.value); }
		else if (key.name == "consoleHeight" && core.serverMode) { camera.res.y = math.stringToInt(key.value); }
		else if (key.name == "enableVertSync") { enableVertSync = math.stringToInt(key.value); }
		else if (key.name == "sampleMod") { sampleMod = (float)math.stringToInt(key.value) / 100.00f; }
		else if (key.name == "framerateLimit") { framerateLimit = math.stringToInt(key.value); }
			// Visual
		else if (key.name == "enableScreenShaders") { enableScreenShaders = math.stringToInt(key.value); }
		else if (key.name == "enableNightShadows") { enableNightShadows = math.stringToInt(key.value); }
		else if (key.name == "enableBetterShadows") { enableBetterShadows = math.stringToInt(key.value); }
		else if (key.name == "enableBetterLight") { enableBetterLight = math.stringToInt(key.value); }
		else if (key.name == "enableTextureSmoothing") { enableTextureSmoothing = math.stringToInt(key.value); }
		else if (key.name == "enableCameraBlur") { enableCameraBlur = math.stringToInt(key.value); }
		else if (key.name == "shadowBlur") { shadowBlur = math.stringToInt(key.value); }
		else if (key.name == "visualUnitHoverColorCode") { visualUnitHoverColor = util.convertUnitHighlightColor(math.stringToInt(key.value)); }
		else if (key.name == "visualUnitHoverColor16") { visualUnitHoverColor = util.parseOldschoolColorCode(key.value); }
			// Gameplay
		else if (key.name == "enableLazyCast") { enableLazyCast = math.stringToInt(key.value); }
		else if (key.name == "enableQuickCast") { enableQuickCast = math.stringToInt(key.value); }
		else if (key.name == "enableMouseScroll") { enableMouseScroll = math.stringToInt(key.value); }
		else if (key.name == "enableDynamicTooltips") { enableDynamicTooltips = math.stringToInt(key.value); }
			// Chat
		else if (key.name == "chatPosX") { chatPosX = math.stringToInt(key.value); }
		else if (key.name == "chatPosY") { chatPosY = math.stringToInt(key.value); }
		else if (key.name == "chatSizeX") { chatSizeX = math.stringToInt(key.value); }
		else if (key.name == "chatSizeY") { chatSizeY = math.stringToInt(key.value); }
		else if (key.name == "chatFontSize") { chatFontSize = math.stringToInt(key.value); }
		else if (key.name == "chatLineSpacing") { chatLineSpacing = math.stringToInt(key.value); }
		else if (key.name == "chatScrollSpeed") { chatScrollSpeed = math.stringToInt(key.value); }
		else if (key.name == "enableChatTimestamps") { enableChatTimestamps = math.stringToInt(key.value); }
			// Console
		else if (key.name == "enableConsoleTimestamps") { enableConsoleTimestamps = math.stringToInt(key.value); }
		else if (key.name == "consoleFontSize") { consoleFontSize = math.stringToInt(key.value); }
		else if (key.name == "consoleLineSpacing") { consoleLineSpacing = math.stringToInt(key.value); }
		else if (key.name == "consoleScrollSpeed") { consoleScrollSpeed = math.stringToInt(key.value); }
			// Audio
		else if (key.name == "enableCameraListener") { enableCameraListener = math.stringToInt(key.value); }
		else if (key.name == "volumeMaster") { volMaster = (float)math.stringToInt(key.value) / 100.00f; }
		else if (key.name == "volumeMusic") { volMusic = (float)math.stringToInt(key.value) / 100.00f; }
		else if (key.name == "volumeEffects") { volEffects = (float)math.stringToInt(key.value) / 100.00f; }
		else if (key.name == "volumeAmbient") { volAmbient = (float)math.stringToInt(key.value) / 100.00f; }
		else if (key.name == "volumeWeather") { volWeather = (float)math.stringToInt(key.value) / 100.00f; }
		else if (key.name == "volumeFootsteps") { volFootsteps = (float)math.stringToInt(key.value) / 100.00f; }
			// Keyboard
		else if (key.name == "hkInventory") { hkInventory = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkActiveItem01") { hkActiveItem[1] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkActiveItem02") { hkActiveItem[2] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkActiveItem03") { hkActiveItem[3] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkActiveItem04") { hkActiveItem[4] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkActiveItem05") { hkActiveItem[5] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkActiveItem06") { hkActiveItem[6] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkActiveItem07") { hkActiveItem[7] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkActiveItem08") { hkActiveItem[8] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkActiveItem09") { hkActiveItem[9] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkCamMoveUp") { hkCamMove[0] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkCamMoveDown") { hkCamMove[1] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkCamMoveLeft") { hkCamMove[2] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkCamMoveRight") { hkCamMove[3] = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkCamToHero") { hkCamToHero = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkChat") { hkChat = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkConsole") { hkConsole = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkDebugMode") { hkDebugMode = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkDebugAdvanced") { hkDebugAdvanced = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkScreenshot") { hkScreenshot = sf::Keyboard::Key(math.stringToInt(key.value)); }
	}

	// Fix the dependancies
	if (enableScreenShaders == 0) { enableBetterShadows = 0; }

	file.close();
}

void cSettings::save()
{
	ofstream file;
	file.open("settings.ini");
	if (file.good())
	{
		file << "[Display]" << "\n";
		file << "screenMode = " << screenMode << "\n";
		file << "screenWidth = " << camera.res.x << "\n";
		file << "screenHeight = " << camera.res.y << "\n";
		file << "enableVertSync = " << enableVertSync << "\n";
		file << "sampleMod = " << math.round(sampleMod * 100.00f) << "\n";
		file << "framerateLimit = " << framerateLimit << "\n";

		file << endl << "[Visual]" << "\n";
		file << "enableScreenShaders = " << enableScreenShaders << "\n";
		file << "enableNightShadows = " << enableNightShadows << "\n";
		file << "enableBetterShadows = " << enableBetterShadows << "\n";
		file << "enableBetterLight = " << enableBetterLight << "\n";
		file << "enableTextureSmoothing = " << enableTextureSmoothing << "\n";
		file << "enableCameraBlur = " << enableCameraBlur << "\n";
		file << "shadowBlur = " << shadowBlur << "\n";

		file << endl << "[Gameplay]" << "\n";
		file << "enableLazyCast = " << enableLazyCast << "\n";
		file << "enableQuickCast = " << enableQuickCast << "\n";
		file << "enableMouseScroll = " << enableMouseScroll << "\n";
		file << "enableDynamicTooltips = " << enableDynamicTooltips << "\n";

		file << endl << "[Chat]" << "\n";
		file << "chatPosX = " << chatPosX << "\n";
		file << "chatPosY = " << chatPosY << "\n";
		file << "chatSizeX = " << chatSizeX << "\n";
		file << "chatSizeY = " << chatSizeY << "\n";
		file << "chatFontSize = " << chatFontSize << "\n";
		file << "chatLineSpacing = " << chatLineSpacing << "\n";
		file << "chatScrollSpeed = " << chatScrollSpeed << "\n";
		file << "enableChatTimestamps = " << enableChatTimestamps << "\n";

		file << endl << "[Console]" << "\n";
		file << "consoleFontSize = " << consoleFontSize << "\n";
		file << "consoleLineSpacing = " << consoleLineSpacing << "\n";
		file << "consoleScrollSpeed = " << consoleScrollSpeed << "\n";
		file << "enableConsoleTimestamps = " << enableConsoleTimestamps << "\n";

		file << endl << "[Audio]" << "\n";
		file << "enableCameraListener = " << enableCameraListener << "\n";
		file << "volumeMaster = " << math.round(volMaster * 100.00f) << "\n";
		file << "volumeMusic = " << math.round(volMusic * 100.00f) << "\n";
		file << "volumeEffects = " << math.round(volEffects * 100.00f) << "\n";
		file << "volumeAmbient = " << math.round(volAmbient * 100.00f) << "\n";
		file << "volumeWeather = " << math.round(volWeather * 100.00f) << "\n";
		file << "volumeFootsteps = " << math.round(volFootsteps * 100.00f) << "\n";

		file << endl << "[Keyboard]" << "\n";
		file << "hkInventory = " << hkInventory << "\n";
		file << "hkActiveItem01 = " << hkActiveItem[1] << "\n";
		file << "hkActiveItem02 = " << hkActiveItem[2] << "\n";
		file << "hkActiveItem03 = " << hkActiveItem[3] << "\n";
		file << "hkActiveItem04 = " << hkActiveItem[4] << "\n";
		file << "hkActiveItem05 = " << hkActiveItem[5] << "\n";
		file << "hkActiveItem06 = " << hkActiveItem[6] << "\n";
		file << "hkActiveItem07 = " << hkActiveItem[7] << "\n";
		file << "hkActiveItem08 = " << hkActiveItem[8] << "\n";
		file << "hkActiveItem09 = " << hkActiveItem[9] << "\n";
		file << "hkCamMoveUp = " << hkCamMove[0] << "\n";
		file << "hkCamMoveDown = " << hkCamMove[1] << "\n";
		file << "hkCamMoveLeft = " << hkCamMove[2] << "\n";
		file << "hkCamMoveRight = " << hkCamMove[3] << "\n";
		file << "hkCamToHero = " << hkCamToHero << "\n";
		file << "hkChat = " << hkChat << "\n";
		file << "hkConsole = " << hkConsole << "\n";
		file << "hkDebugMode = " << hkDebugMode << "\n";
		file << "hkDebugAdvanced = " << hkDebugAdvanced << "\n";
		file << "hkScreenshot = " << hkScreenshot << "\n";
	}
	else { console.error << "[ERROR] Can't save the settings file!" << "\n"; }
	file.close();
}