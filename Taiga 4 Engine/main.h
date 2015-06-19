
#include "precomp.h"
#include "define.h"

#define endl	"\n"

typedef sf::Vector2f		vec;
typedef sf::Vector2f		vec2;
typedef sf::Vector2f		vec2f;
typedef sf::Vector2i		vec2i;
typedef std::string			str;
typedef sf::Color			color;

#include "math.h"
#include "util.h"
#include "console.h"
#include "window.h"
#include "visual.h"
#include "icon.h"
#include "refSystem.h"
#include "audio.h"
#include "item.h"
#include "UI.h"
#include "path.h"
#include "ai.h"
#include "animation.h"
#include "unit.h"
#include "game.h"
#include "database.h"
#include "craft.h"
#include "client.h"
#include "server.h"
#include "camera.h"
#include "settings.h"
#include "saveload.h"
#include "world.h"
#include "overworld.h"
#include "editor.h"
#include "script.h"
#include "api.h"

using namespace std;

class cCore
{
public:
	int videoCard;
	float timeModifier;

	int menuState;
	bool serverMode;
	bool localServer;

	bool shutdown;
	bool debugMode;
	bool advancedDebug;
	bool editorMode;

	int thread_antifreeze[16];
	bool thread_shutdown[16];
	int thread_windowTicks;
	int thread_serverWorldTicks;
	int thread_windowTicksPerSec;
	int thread_serverWorldTicksPerSec;

	// Initializing
	cCore() {
		videoCard = VIDEO_OTHER;
		timeModifier = 1.00f;
		menuState = STATE_GAME;
		serverMode = false;
		localServer = false;

		shutdown = false;
		debugMode = false;
		advancedDebug = false;
		editorMode = false;

		thread_windowTicks = 0;
		thread_serverWorldTicks = 0;
		for (int i = 0; i < 16; i++) { thread_shutdown[i] = false; thread_antifreeze[i] = 0; }
	}
};

extern cCore core;
extern cValue value;
extern cMath math;
extern cConsole console;
extern cWindow window;
extern cVisual visual;
extern cAudio audio;
extern cUI ui;
extern cPath path;
extern cGame game;
extern cDatabase database;
extern cCraft craft;
extern cServer server;
extern cClient client;
extern cCamera camera;
extern cSettings settings;
extern cSave save;
extern cWorld world;
extern cOverworld overworld;
extern cEditor editor;
extern cUtil util;
extern cScript script;
extern cAI ai;
extern cAPI api;