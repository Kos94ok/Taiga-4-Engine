
#include "define.h"

class cSettingsKey
{
public:
	std::string name;
	std::string value;
};

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
	int enableBetterShadows;
	// Dynamic lighting [Enables realistic lighting system]
	int enableDynamicLight;
	// Texture filtering [Reduces image pixelization]
	int enableTextureSmoothing;
	// Camera blur [Game world is blurred when the camera is moving]
	int enableCameraBlur;
	// Vertical sync [Fixes the image tearing]
	int enableVertSync;
	// Quick cast [Active items trigger instantly]
	int enableQuickCast;
	// Mouse scroll [Mouse outside of the window moves the camera]
	int enableMouseScroll;
	// Dynamic tooltips [Tooltips are moving with the mouse]
	int enableDynamicTooltips;

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

	// Starting path counter [Increases the open area close to spawn point]
	int wgStartingPath;
	// Minimal path length [Increases the total map size]
	int wgMinimalPathLength;
	// Maximum path length [Decreases the maximum map size]
	int wgMaximalPathLength;
	// Path fork chance [Increases the amount of paths on the map]
	int wgPathForkChance;

	// Camera listener [Positional sound is calculated from camera position]
	int enableCameraListener;
	// Master volume
	float volMaster;
	// Music volume
	float volMusic;
	// Special effect volume
	float volEffects;
	// Ambient sound volume
	float volAmbient;
	// Weather sound volume
	float volWeather;
	// Footstep volume
	float volFootsteps;

	// Day-night cycle speed
	float wdDayNightSpeed;
	// Night transition speed
	float wdNightChangeSpeed;

	// Enable console timestamps
	int enableConsoleTimestamps;
	// Console scroll speed
	int consoleScrollSpeed;
	// Console font size
	int consoleFontSize;
	// Console line spacing
	int consoleLineSpacing;

	// Hotkeys
	sf::Keyboard::Key hkConsole;
	sf::Keyboard::Key hkDebugMode;
	sf::Keyboard::Key hkDebugAdvanced;
	sf::Keyboard::Key hkInventory;
	sf::Keyboard::Key hkActiveItem[LIMIT_ACTIVEBUTTONS];
	sf::Keyboard::Key hkCamMove[4];
	sf::Keyboard::Key hkCamToHero;

	void load();
	void save();
	void setDefault();

	bool getNextKey(std::ifstream* file, cSettingsKey *key);

	// Graphics presets
	void setUltra();
};