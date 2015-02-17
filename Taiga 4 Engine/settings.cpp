
#include "main.h"

void cSettings::setDefault()
{
	// 0 - Low
	// 1 - Medium
	// 2 - High
	// 3 - Ultra
	this->unitDepthCheck = 3;				// Fixed value
	this->unitRenderDistance = 2;			// Fixed value

	// 0 - Low / Disabled
	// 1 - High / Enabled
	this->enableScreenShaders = 1;
	this->enableNightShadows = 1;
	this->enableBetterShadows = 1;
	this->enableDynamicLight = 1;			// Only disabled if shaders are not supported
	this->enableTextureSmoothing = 0;
	this->enableCameraBlur = 1;
	this->enableVertSync = 1;
	this->enableQuickCast = 1;

	// Any value
	this->screenMode = 1;					// 0 - Window, 1 - Borderless, 2 - Fullscreen
	this->pixelization = 5000.00f;			// 5000+ - Disabled
	this->shadowBlur = 1;					// 1 - Disabled, 2+ - Shadow sample count	[Incompatible with better shadows]
	this->antialiasingLevel = 0;			// 0 - Disabled, 1+ - AA level
	this->sampleMod = 1.00f;				// 0.50 - Subsampling, 1.00 - Default, 2.00 - Supersampling

	// World generation
	this->wgStartingPath = 1;				// Amount of paths
	this->wgMinimalPathLength = 3;			// Minimal amount of steps before termination
	this->wgMaximalPathLength = 10;			// Maximum amount of steps before termination
	this->wgPathForkChance = 20;			// Chance to create another path on every step (percentage)

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
	this->hkDebugMode = sf::Keyboard::F3;
	this->hkDebugAdvanced = sf::Keyboard::F4;

	// Garbage
	camera.res.x = window.getScreenSize().x;
	camera.res.y = window.getScreenSize().y;
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
	this->pixelization = 5000.00f;
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
	if (!file.good()) { cout << "[ERROR] Can't open the settings file. Returning to default" << "\n"; return; }
	
	// Looking for values until the end of the file
	while (retVal)
	{
		retVal = getNextKey(&file, &key);
		// Matching the values
		if (key.name == "enableScreenShaders") { enableScreenShaders = math.stringToInt(key.value); }
		else if (key.name == "enableNightShadows") { enableNightShadows = math.stringToInt(key.value); }
		else if (key.name == "enableBetterShadows") { enableBetterShadows = math.stringToInt(key.value); }
		else if (key.name == "enableTextureSmoothing") { enableTextureSmoothing = math.stringToInt(key.value); }
		else if (key.name == "enableCameraBlur") { enableCameraBlur = math.stringToInt(key.value); }
		else if (key.name == "enableVertSync") { enableVertSync = math.stringToInt(key.value); }
		else if (key.name == "enableQuickCast") { enableQuickCast = math.stringToInt(key.value); }

		else if (key.name == "screenMode") { screenMode = math.stringToInt(key.value); }
		else if (key.name == "pixelization") { pixelization = (float)math.stringToInt(key.value); }
		else if (key.name == "shadowBlur") { shadowBlur = math.stringToInt(key.value); }
		else if (key.name == "sampleMod") { sampleMod = (float)math.stringToInt(key.value) / 100.00f; }

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
		else if (key.name == "hkDebugMode") { hkDebugMode = sf::Keyboard::Key(math.stringToInt(key.value)); }
		else if (key.name == "hkDebugAdvanced") { hkDebugAdvanced = sf::Keyboard::Key(math.stringToInt(key.value)); }

		else if (key.name == "screenWidth") { camera.res.x = math.stringToInt(key.value); }
		else if (key.name == "screenHeight") { camera.res.y = math.stringToInt(key.value); }
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

		file << endl << "[Visual]" << "\n";
		file << "enableScreenShaders = " << enableScreenShaders << "\n";
		file << "enableNightShadows = " << enableNightShadows << "\n";
		file << "enableBetterShadows = " << enableBetterShadows << "\n";
		file << "enableTextureSmoothing = " << enableTextureSmoothing << "\n";
		file << "enableCameraBlur = " << enableCameraBlur << "\n";
		file << "pixelization = " << math.round(pixelization) << "\n";
		file << "shadowBlur = " << shadowBlur << "\n";

		file << endl << "[Gameplay]" << "\n";
		file << "enableQuickCast = " << enableQuickCast << "\n";

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
		file << "hkDebugMode = " << hkDebugMode << "\n";
		file << "hkDebugAdvanced = " << hkDebugAdvanced << "\n";
	}
	else { cout << "[ERROR] Can't save the settings file!" << "\n"; }
	file.close();
}