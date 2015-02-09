
#include "define.h"

class cSettings
{
public:
	// Unit depth [Higher values reduce unit flickering]
	int unitDepthCheck;
	// Render distance [Objects' shadows may not be visible sometimes on lower values]
	int unitRenderDistance;

	// Fullscreen shaders [Required for less important effects]
	int enableScreenShaders;
	// Night shadows [Moon can also cast objects' shadows]
	int enableNightShadows;
	// Alternate shadows [Prevents shadows from stacking]
	// Warning: Animation glitch, do not release
	int enableBetterShadows;
	// Texture filtering [Reduces image pixelization]
	int enableTextureSmoothing;
	// Camera blur [Game world is blurred when moving the camera]
	int enableCameraBlur;
	// Vertical sync [Fixes the image tearing]
	int enableVertSync;
	// Quick cast [Active items trigger instantly]
	int enableQuickCast;

	// Screen mode [Fullscreen / Window / Borderless]
	int screenMode;
	// Pixelization [Uses shaders to visually decrease resolution]
	float pixelization;
	// Shadow blur [Uses shaders to make shadows look more realistic]
	int shadowBlur;
	// Antialiasing [Hardware-dependant type of AA, improves image quality]
	int antialiasingLevel;
	// Sample modifier [Render resolution over display resolution]
	float sampleMod;

	// Hotkeys
	sf::Keyboard::Key hkDebugMode;
	sf::Keyboard::Key hkDebugAdvanced;
	sf::Keyboard::Key hkInventory;
	sf::Keyboard::Key hkActiveItem[LIMIT_ACTIVEBUTTONS];
	sf::Keyboard::Key hkCamMove[4];

	void setDefault();

	// Graphics presets
	void setUltra();
};