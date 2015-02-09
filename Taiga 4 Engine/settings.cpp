
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
	this->enableScreenShaders = 0;
	this->enableNightShadows = 1;
	this->enableBetterShadows = 0;
	this->enableTextureSmoothing = 1;
	this->enableCameraBlur = 0;
	this->enableVertSync = 1;
	this->enableQuickCast = 0;

	// Any value
	this->screenMode = 0;					// 0 - Window, 1 - Borderless, 2 - Fullscreen
	this->pixelization = 5000.00f;			// 5000+ - Disabled
	this->shadowBlur = 1;					// 1 - Disabled, 2+ - Shadow sample count
	this->antialiasingLevel = 0;			// 0 - Disabled, 1+ - AA level
	this->sampleMod = 1.00f;				// 0.50 - Subsampling, 1.00 - Default, 2.00 - Supersampling

	// Hotkeys
	this->hkInventory = sf::Keyboard::E;
		// Buttons [1; LIMIT) reserved for the items / abilities
	this->hkActiveItem[1] = sf::Keyboard::Num1;
	this->hkActiveItem[2] = sf::Keyboard::Num2;
	this->hkActiveItem[3] = sf::Keyboard::Num3;
	this->hkActiveItem[4] = sf::Keyboard::Num4;
		// Up, Down, Left, Right
	this->hkCamMove[0] = sf::Keyboard::W;
	this->hkCamMove[1] = sf::Keyboard::S;
	this->hkCamMove[2] = sf::Keyboard::A;
	this->hkCamMove[3] = sf::Keyboard::D;
	this->hkDebugMode = sf::Keyboard::F3;
	this->hkDebugAdvanced = sf::Keyboard::F4;

	// Garbage
	camera.res.x = 1366;
	camera.res.y = 600;
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