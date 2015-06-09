
#include "main.h"

cCore core;
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

int main(int argc, char* argv[])
{
	game.access.lock();
	cout << "[MAIN] Main thread started" << "\n";
	cout << "[MAIN] Parsing arguments [" << argc << "]" << "\n";
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-server") == 0) { core.serverMode = true; }
	}
	// Random initialization
	cout << "[MAIN] Initializing" << "\n";
	//setlocale(LC_ALL, "");
	//setlocale(LC_ALL, "Russian");
	srand(time(0));
	timeBeginPeriod(5);
	if (!core.serverMode) { core.localServer = true; }
	util.detectVideoCard();

	// Loading databases
	cout << "[MAIN] Loading databases" << "\n";
	settings.setDefault();
	settings.load();
	database.init();
	craft.loadRecipes();
	visual.init();
	world.analyzeBlueprints();

	// Starting the threads
	cout << "[MAIN] Starting the threads\n";
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
		cout << "[MAIN] Making some menu" << "\n";
		//ui.element[ui.getElementId(id)].s
		int id = ui.addElement("image", vec2f(camera.res.x / 2, camera.res.y / 2));
		ui.element[ui.getElementId(id)].texture = visual.addTexture("bg_art.png");
		ui.element[ui.getElementId(id)].size = vec2f(camera.res.x, camera.res.y);
		id = ui.createText(vec2f(camera.res.x / 2, camera.res.y / 2 - 70), "Taiga Survival Indev", "That is a tooltip!");
		ui.element[ui.getElementId(id)].ignoreOrigin = false;
		//ui.element[ui.getElementId(id)].tooltip.pos
		id = ui.addElement("button_test", sf::Vector2f(camera.res.x / 2.00f, camera.res.y / 2.00f));
		ui.element[ui.getElementId(id)].setText("Taiga Mini");
		ui.element[ui.getElementId(id)].button.action = "start_taigaMini";
		id = ui.addElement("button_test", sf::Vector2f(camera.res.x / 2.00f, camera.res.y / 2.00f + 35.00f));
		ui.element[ui.getElementId(id)].setText("Editor");
		ui.element[ui.getElementId(id)].button.action = "start_editor";
		id = ui.addElement("button_test", sf::Vector2f(camera.res.x / 2.00f, camera.res.y / 2.00f + 70.00f));
		ui.element[ui.getElementId(id)].setText("Taiga Maxi");
		ui.element[ui.getElementId(id)].button.action = "start_taigaMaxi";
		id = ui.addElement("button_test", sf::Vector2f(camera.res.x / 2.00f, camera.res.y / 2.00f + 105.00f));
		ui.element[ui.getElementId(id)].setText("Quick Connect");
		ui.element[ui.getElementId(id)].button.action = "connect_temp";
	}
	game.access.unlock();

	ai.enable();
	script.execute(cScript::spawnEnemies, 0);
	//script.execute(cScript::test_unitAddSystem, 0);

	cout << "[MAIN] Overlooking the threads..." << "\n";
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
			if (!(core.serverMode && i == 0)) { core.thread_antifreeze[i] += 1; }
			if (core.thread_antifreeze[i] > 1000) {
				core.thread_antifreeze[i] = 0;
				cout << "[MAIN] Thread " << i << " appears to be frozen..." << endl;
			}
		}
		Sleep(1);
	}

	// Waiting for shutdown
	cout << "[MAIN] Waiting for threads to finish..." << "\n";
	threadWindow.join();
	threadServerWorld.join();
	threadServerConnect.join();
	threadServerReceive.join();
	threadServerSend.join();
	threadClientReceive.join();
	threadClientSend.join();
	threadWorldLoader.join();
	threadAICore.join();
	cout << "[MAIN] Waiting for script threads to finish..." << "\n";
	for (int i = 0; i < (int)script.threadVector.size(); i++)
	{
		script.threadVector[i].join();
	}
	// Terminating the blocking threads
	cout << "[MAIN] Terminating blocking threads..." << "\n";
	if (console.online) { TerminateThread(threadConsole.native_handle(), 0); }
	threadConsole.join();

	cout << "[MAIN] Cleaning up" << "\n";

	// Cleaning up
	return 0;
}