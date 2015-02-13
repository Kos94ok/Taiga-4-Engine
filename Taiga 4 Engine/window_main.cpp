
#include "main.h"

void windowMain()
{
	if (core.serverMode) { return; }

	cout << "[WND] Window thread started" << endl;
	srand(time(0));
	sf::Uint32 screenMode;
	sf::ContextSettings context;
	context.antialiasingLevel = settings.antialiasingLevel;
	if (settings.screenMode == 0) { screenMode = sf::Style::Default; }
	else if (settings.screenMode == 1) {
		screenMode = sf::Style::None;
		camera.res.x = window.getScreenSize().x;
		camera.res.y = window.getScreenSize().y;
	}
	else if (settings.screenMode == 2) { screenMode = sf::Style::Fullscreen; }
	window.winHandle.create(sf::VideoMode(camera.res.x, camera.res.y), "Taiga 4 Engine", screenMode, context);
	window.texHandle.create(camera.res.x * settings.sampleMod, camera.res.y * settings.sampleMod);
	window.texHandleShadow.create(camera.res.x * settings.sampleMod, camera.res.y * settings.sampleMod);
	window.texHandleLight.create(camera.res.x * settings.sampleMod, camera.res.y * settings.sampleMod);
	window.texHandleTop.create(camera.res.x * settings.sampleMod, camera.res.y * settings.sampleMod);
	window.winHandle.setVerticalSyncEnabled(math.intToBool(settings.enableVertSync));
	window.updateAspectRatio();
	
	cout << "[WND] Starting the main loop" << endl;
	while (!core.shutdown)
	{
		window.mainEvent();
		window.mainPaint();
	}
	window.winHandle.close();
	cout << "[WND] Cleaning up" << endl;
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