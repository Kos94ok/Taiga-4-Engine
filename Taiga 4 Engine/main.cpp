
#include "main.h"

cCore core;
cValue value;
cMath math;
cConsole console;
cWindow window;
cVisual visual;
cAudio audio;
cUI ui;
cPath path;
cGame game;
cDatabase database;
cCraft craft;
cServer server;
cClient client;
cCamera camera;
cSettings settings;
cSave save;
cWorld world;
cEditor editor;
cUtil util;
cScript script;
cAI ai;
cAPI api;

/*
TODO:
- Script message (event) system?
- Multiple levels of enemies based on the Slender-style system
*/

int main(int argc, char* argv[])
{
	game.access.lock();
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-server") == 0) { core.serverMode = true; }
	}
	console << "[MAIN] Main thread started" << "\n";
	console << "[MAIN] Parsed [" << argc << "] arguments" << "\n";
	// Random initialization
	console << "[MAIN] Initializing" << "\n";
	//setlocale(LC_ALL, "");
	//setlocale(LC_ALL, "Russian");
	srand(time(0));
	timeBeginPeriod(5);
	if (!core.serverMode) { core.localServer = true; }
	util.detectVideoCard();

	// Loading databases
	console << "[MAIN] Loading databases" << "\n";
	settings.setDefault();
	settings.load();
	database.init();
	craft.loadRecipes();
	visual.init();
	world.analyzeBlueprints();

	// Starting the threads
	console << "[MAIN] Starting the threads\n";
	thread threadWindow(windowMain);
	Sleep(10);
	thread threadServerWorld(serverWorldMain);
	Sleep(10);
	thread threadServerConnect(serverConnectMain);
	Sleep(10);
	thread threadServerReceive(serverReceiveMain);
	Sleep(10);
	thread threadServerSend(serverSendMain);
	Sleep(10);
	thread threadClientReceive(clientReceiveMain);
	Sleep(10);
	thread threadClientSend(clientSendMain);
	Sleep(10);
	thread threadWorldLoader(worldLoaderMain);
	Sleep(10);
	thread threadAICore(AICoreMain);
	Sleep(10);
	
	// Initializing the server
	if (core.serverMode)
	{
		server.initialize();
	}
	else
	{
		// Creating the menu
		console << "[MAIN] Making some menu" << "\n";
		//ui.element[ui.getElementId(id)].s
		int id = ui.addElement("image", vec2f(camera.res.x / 2, camera.res.y / 2));
		ui.element[ui.getElementId(id)].texture = visual.addTexture("bg_art.png");
		ui.element[ui.getElementId(id)].size = vec2f(camera.res.x, camera.res.y);
		id = ui.createText(vec2f(camera.res.x / 2, camera.res.y / 2 - 70), "Taiga Survival Alpha v0.10", "That is a tooltip!");
		ui.element[ui.getElementId(id)].ignoreOrigin = false;
		//ui.element[ui.getElementId(id)].tooltip.pos
		id = ui.addElement("button_test", sf::Vector2f(camera.res.x / 2.00f, camera.res.y / 2.00f + 0.00f));
		ui.element[ui.getElementId(id)].setText("Taiga Mini");
		ui.element[ui.getElementId(id)].button.action = "start_taigaMaxi";
		id = ui.addElement("button_test", sf::Vector2f(camera.res.x / 2.00f, camera.res.y / 2.00f + 35.00f));
		ui.element[ui.getElementId(id)].setText("Generic Shooter");
		ui.element[ui.getElementId(id)].button.action = "start_genericShooter";
		id = ui.addElement("button_test", sf::Vector2f(camera.res.x / 2.00f, camera.res.y / 2.00f + 70.00f));
		ui.element[ui.getElementId(id)].setText("Editor");
		ui.element[ui.getElementId(id)].button.action = "start_editor";
		id = ui.addElement("button_test", sf::Vector2f(camera.res.x / 2.00f, camera.res.y / 2.00f + 105.00f));
		ui.element[ui.getElementId(id)].setText("Quick Connect");
		ui.element[ui.getElementId(id)].button.action = "connect_temp";
	}
	game.access.unlock();

	ai.enable();

	console << "[MAIN] Overlooking the threads..." << "\n";
	int globalTime = timeGetTime();
	while (!core.shutdown)
	{
		// Counting ticks
		if (timeGetTime() - globalTime >= 1000)
		{
			globalTime = timeGetTime();
			core.thread_windowTicksPerSec = core.thread_windowTicks;
			core.thread_serverWorldTicksPerSec = core.thread_serverWorldTicks;
			core.thread_windowTicks = 0;
			core.thread_serverWorldTicks = 0;
		}
		// Adding some antifreeze
		for (int i = 0; i < 9; i++) {
			if (!(core.serverMode && (i == 5 || i == 6))) { core.thread_antifreeze[i] += 1; }
			if (core.thread_antifreeze[i] > 1000) {
				core.thread_antifreeze[i] = 0;
				console.error << "[MAIN] Thread " << i << " appears to be frozen..." << endl;
			}
		}
		Sleep(1);
	}

	// Waiting for shutdown
	console << "[MAIN] Waiting for script threads to finish..." << "\n";
	for (int i = 0; i < (int)script.threadVector.size(); i++)
	{
		script.threadVector[i].join();
	}
	console << "[MAIN] Waiting for core threads to finish..." << "\n";
	core.thread_shutdown[8] = true;		threadAICore.join();
	core.thread_shutdown[7] = true;		threadWorldLoader.join();
	core.thread_shutdown[6] = true;		threadClientSend.join();
	core.thread_shutdown[5] = true;		threadClientReceive.join();
	core.thread_shutdown[4] = true;		threadServerSend.join();
	core.thread_shutdown[3] = true;		threadServerReceive.join();
	core.thread_shutdown[2] = true;		threadServerConnect.join();
	core.thread_shutdown[1] = true;		threadServerWorld.join();
	core.thread_shutdown[0] = true;		threadWindow.join();

	console << "[MAIN] Cleaning up" << endl << endl << endl;
	// Cleaning up
	return 0;
}