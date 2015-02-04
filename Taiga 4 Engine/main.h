
#include "precomp.h"
#include "define.h"

typedef sf::Vector2f		vec;
typedef sf::Vector2f		vec2;
typedef sf::Vector2f		vec2f;
typedef sf::Vector2i		vec2i;

#include "math.h"
#include "util.h"
#include "console.h"
#include "window.h"
#include "visual.h"
#include "refSystem.h"
#include "item.h"
#include "UI.h"
#include "path.h"
#include "unit.h"
#include "game.h"
#include "database.h"
#include "craft.h"
#include "client.h"
#include "server.h"
#include "camera.h"
#include "settings.h"
#include "world.h"

using namespace std;

class cCore
{
public:
	float timeModifier;

	int menuState;
	bool serverMode;
	bool localServer;

	bool shutdown;
	bool debugMode;
	bool advancedDebug;

	int thread_windowTicks;
	int thread_serverWorldTicks;
	int thread_windowTicksPerSec;
	int thread_serverWorldTicksPerSec;

	// Initializing
	cCore() {
		timeModifier = 1.00f;
		menuState = STATE_GAME;
		serverMode = false;
		localServer = false;

		shutdown = false;
		debugMode = false;
		advancedDebug = false;

		thread_windowTicks = 0;
		thread_serverWorldTicks = 0;
	}
};

extern cCore core;
extern cMath math;
extern cConsole console;
extern cWindow window;
extern cVisual visual;
extern cUI ui;
extern cPath path;
extern cGame game;
extern cDatabase database;
extern cCraft craft;
extern cServer server;
extern cClient client;
extern cCamera camera;
extern cSettings settings;
extern cWorld world;