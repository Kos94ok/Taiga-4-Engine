
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
cWorld world;
cEditor editor;
cAPI api;

int main(int argc, char* argv[])
{
	cout << "[MAIN] Main thread started" << endl;
	cout << "[MAIN] Parsing arguments [" << argc << "]" << endl;
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-server") == 0) { core.serverMode = true; }
	}
	// Random initialization
	cout << "[MAIN] Initializing" << endl;
	//setlocale(LC_ALL, "");
	//setlocale(LC_ALL, "Russian"); 
	srand(time(0));
	timeBeginPeriod(5);
	if (!core.serverMode) { core.localServer = true; }

	// Loading databases
	cout << "[MAIN] Loading databases" << endl;
	settings.setDefault();
	database.init();
	craft.loadRecipes();
	visual.init();

	// Starting the threads
	cout << "[MAIN] Starting the threads" << endl;
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
	thread threadConsole(consoleMain);
	Sleep(50);

	cout << "[MAIN] Making some menu" << endl;
	int id = ui.addElement("button_test", sf::Vector2f(camera.res.x / 2.00f, camera.res.y / 2.00f));
	ui.element[ui.getElementId(id)].setText("Taiga Mini");
	ui.element[ui.getElementId(id)].button.action = "start_taigaMini";
	id = ui.addElement("button_test", sf::Vector2f(camera.res.x / 2.00f, camera.res.y / 2.00f + 35.00f));
	ui.element[ui.getElementId(id)].setText("Editor");
	ui.element[ui.getElementId(id)].button.action = "start_editor";
	id = ui.addElement("button_test", sf::Vector2f(camera.res.x / 2.00f, camera.res.y / 2.00f + 70.00f));
	ui.element[ui.getElementId(id)].setText("Arena");
	ui.element[ui.getElementId(id)].button.action = "start_arena";
	id = ui.addElement("button_test", sf::Vector2f(camera.res.x / 2.00f, camera.res.y / 2.00f + 105.00f));
	ui.element[ui.getElementId(id)].setText("88.85.144.209");
	ui.element[ui.getElementId(id)].button.action = "connect_temp";

	cout << "[MAIN] Overlooking the threads..." << endl;
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
	cout << "[MAIN] Waiting for threads to finish..." << endl;
	threadWindow.join();
	threadServerWorld.join();
	threadServerConnect.join();
	threadServerReceive.join();
	threadServerSend.join();
	threadClientReceive.join();
	threadClientSend.join();
	// Terminating the blocking threads
	if (console.online) { TerminateThread(threadConsole.native_handle(), 0); }
	threadConsole.join();

	cout << "[MAIN] Cleaning up" << endl;

	// Cleaning up
	return 0;
}