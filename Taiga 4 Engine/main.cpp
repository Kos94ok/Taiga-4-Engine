
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
cAPI api;

int main(int argc, char* argv[])
{
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
	Sleep(50);
	thread threadServerWorld(serverWorldMain);
	Sleep(50);
	thread threadServerConnect(serverConnectMain);
	Sleep(50);
	thread threadServerReceive(serverReceiveMain);
	Sleep(50);
	thread threadServerSend(serverSendMain);
	Sleep(50);
	thread threadClientReceive(clientReceiveMain);
	Sleep(50);
	thread threadClientSend(clientSendMain);
	Sleep(50);
	thread threadWorldLoader(worldLoaderMain);
	Sleep(50);
	thread threadConsole(consoleMain);
	Sleep(50);

	cout << "[MAIN] Making some menu" << "\n";
	int id = ui.addElement("button_test", sf::Vector2f(camera.res.x / 2.00f, camera.res.y / 2.00f));
	ui.element[ui.getElementId(id)].setText("Taiga Mini");
	ui.element[ui.getElementId(id)].button.action = "start_taigaMini";
	id = ui.addElement("button_test", sf::Vector2f(camera.res.x / 2.00f, camera.res.y / 2.00f + 35.00f));
	ui.element[ui.getElementId(id)].setText("Editor");
	ui.element[ui.getElementId(id)].button.action = "start_editor";
	id = ui.addElement("button_test", sf::Vector2f(camera.res.x / 2.00f, camera.res.y / 2.00f + 70.00f));
	ui.element[ui.getElementId(id)].setText("Map Gen Test");
	ui.element[ui.getElementId(id)].button.action = "start_arena";
	id = ui.addElement("button_test", sf::Vector2f(camera.res.x / 2.00f, camera.res.y / 2.00f + 105.00f));
	ui.element[ui.getElementId(id)].setText("Quick Connect");
	ui.element[ui.getElementId(id)].button.action = "connect_temp";

	cout << "[MAIN] Overlooking the threads..." << "\n";
	int globalTime = timeGetTime();
	while (!core.shutdown)
	{
		if (timeGetTime() - globalTime >= 1000)
		{
			globalTime = timeGetTime();
			core.thread_windowTicksPerSec = core.thread_windowTicks;
			core.thread_serverWorldTicksPerSec = core.thread_serverWorldTicks;
			core.thread_windowTicks = 0;
			core.thread_serverWorldTicks = 0;
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
	// Terminating the blocking threads
	if (console.online) { TerminateThread(threadConsole.native_handle(), 0); }
	threadConsole.join();

	cout << "[MAIN] Cleaning up" << "\n";

	// Cleaning up
	return 0;
}