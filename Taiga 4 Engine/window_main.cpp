
#include "main.h"

void windowMain()
{
	int threadId = 0;
	if (core.serverMode) { return; }

	cout << "[WND] Window thread started" << "\n";
	srand(time(0));
	sf::Uint32 screenMode;
	sf::ContextSettings context;
	context.antialiasingLevel = settings.antialiasingLevel;
	// Setting up window options
	if (settings.screenMode == 0) { screenMode = sf::Style::Default; }
	else if (settings.screenMode == 1) {
		screenMode = sf::Style::None;
		camera.res.x = window.getScreenSize().x;
		camera.res.y = window.getScreenSize().y;
	}
	else if (settings.screenMode == 2) { screenMode = sf::Style::Fullscreen; }
	// Checking the max texture size
	bool resChanged = false;
	if (visual.maxTextureSize < camera.res.x) { camera.res.x = visual.maxTextureSize; resChanged = true; }
	if (visual.maxTextureSize < camera.res.y) { camera.res.y = visual.maxTextureSize; resChanged = true; }
	if (resChanged) {
		window.showWarning(MSG_TEXTURESIZE_LIMIT);
		if (settings.sampleMod > 1.00f) { settings.sampleMod = 1.00f; }
	}
	// Creating the window
	window.winHandle.create(sf::VideoMode(camera.res.x, camera.res.y), "Taiga 4 Engine", screenMode, context);
	// Creating the textures
	window.texHandle.create(camera.res.x * settings.sampleMod, camera.res.y * settings.sampleMod);
	window.texHandleShadow.create(camera.res.x * settings.sampleMod, camera.res.y * settings.sampleMod);
	window.texHandleLight.create(camera.res.x * settings.sampleMod, camera.res.y * settings.sampleMod);
	window.texHandleTop.create(camera.res.x * settings.sampleMod, camera.res.y * settings.sampleMod);
	window.winHandle.setVerticalSyncEnabled(math.intToBool(settings.enableVertSync));
	window.updateAspectRatio();
	
	cout << "[WND] Starting the main loop" << "\n";
	while (!core.shutdown)
	{
		window.mainEvent();
		window.mainPaint();
		core.thread_antifreeze[threadId] = 0;
	}
	window.winHandle.close();
	cout << "[WND] Cleaning up" << "\n";
}

sf::Vector2f cWindow::getMousePos(bool useMatrix)
{
	sf::Vector2i mousePosI = sf::Mouse::getPosition(winHandle);
	sf::Vector2f mousePos(mousePosI.x, mousePosI.y);

	// Screen resize fix
	mousePos.x *= (float)camera.res.x / (float)winHandle.getSize().x;
	mousePos.y *= (float)camera.res.y / (float)winHandle.getSize().y;

	if (useMatrix)
	{
		// Sample mod fix
		mousePos.x *= settings.sampleMod;
		mousePos.y *= settings.sampleMod;
		mousePos = matrixHandle.getInverse().transformPoint(mousePos);
	}
	return mousePos;
}

sf::Vector2f cWindow::getScreenSize()
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	return sf::Vector2f(desktop.right, desktop.bottom);
}
