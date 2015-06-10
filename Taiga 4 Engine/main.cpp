
#include "main.h"

cCore core;
cValue value;
cMath math;
cConsole console;
cWindow window;
cVisual visual;
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
- Stop time for Generic Shooter (at night)
- Add light to enemies
- Make enemies attack the hero
*/

int main(int argc, char* argv[])
{
	game.access.lock();
	console << "[MAIN] Main thread started" << "\n";
	console << "[MAIN] Parsing arguments [" << argc << "]" << "\n";
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-server") == 0) { core.serverMode = true; }
	}
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
	thread threadConsoleOutput(consoleOutputMain);
	Sleep(10);
	thread threadConsole(consoleMain);
	Sleep(10);
	
	// Initializing the server
	if (core.serverMode)
	{
		server.initialize();
	}
	else
	{
		// Hiding the console if needed
		if (!console.displayed) { console.hide(); }
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
		for (int i = 0; i < 10; i++) {
			if (!(core.serverMode && i == 0)) { core.thread_antifreeze[i] += 1; }
			if (core.thread_antifreeze[i] > 1000) {
				core.thread_antifreeze[i] = 0;
				console << "[MAIN] Thread " << i << " appears to be frozen..." << endl;
			}
		}
		Sleep(1);
	}

	// Waiting for shutdown
	console << "[MAIN] Waiting for threads to finish..." << "\n";
	threadWindow.join();
	threadServerWorld.join();
	threadServerConnect.join();
	threadServerReceive.join();
	threadServerSend.join();
	threadClientReceive.join();
	threadClientSend.join();
	threadWorldLoader.join();
	threadAICore.join();
	threadConsoleOutput.join();
	console << "[MAIN] Waiting for script threads to finish..." << "\n";
	for (int i = 0; i < (int)script.threadVector.size(); i++)
	{
		script.threadVector[i].join();
	}
	// Terminating the blocking threads
	console << "[MAIN] Terminating blocking threads..." << "\n";
	if (console.online) { TerminateThread(threadConsole.native_handle(), 0); }
	threadConsole.join();

	console << "[MAIN] Cleaning up" << "\n";

	// Cleaning up
	return 0;
}