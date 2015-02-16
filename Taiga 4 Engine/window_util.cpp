
#include "main.h"

void cWindow::showWarning(int msg, bool critical)
{
	string message = "Unknown warning";
	if (msg == MSG_NOSHADERSUPPORT) { 
		message = "Shaders are not supported on your system. Lighting system and some other features are disabled.";
	}
	else if (msg == MSG_SHADERCOMPILE_UNIVERSAL) {
		message = "Universal shader compilation failed. Shadows are forced to \"low\". Some effects are disabled.";
	}
	else if (msg == MSG_SHADERCOMPILE_LIGHT) {
		message = "Light shader compilation failed. Lighting system is forced to \"low\".";
	}
	else if (msg == MSG_SHADERCOMPILE_OTHER) {
		message = "One or more shaders are failed to compile. Some effects may be lost";
	}
	else if (msg == MSG_TEXTURESIZE_LIMIT) {
		message = "Maximum texture size reached. Resolution will be reduced.";
	}
	
	// Windows routine
#ifdef SYSTEM_WINDOWS
	MessageBoxA(window.winHandle.getSystemHandle(), message.c_str(), "Warning", MB_ICONWARNING | MB_OK);
#endif
}

void cWindow::updateAspectRatio()
{
	float minX = 0.00f;
	float minY = 0.00f;
	float maxX = camera.res.x;
	float maxY = camera.res.y;
	
	winHandle.setView(sf::View(sf::FloatRect(minX, minY, maxX, maxY)));
}
