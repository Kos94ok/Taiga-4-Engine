
#include "precomp.h"

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

	// Some functions
	bool serverSide() {
		return (serverMode || localServer);
	}
};

extern cCore core;