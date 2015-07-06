
#include "UI.h"
#include "chat.h"
#include "camera.h"
#include "visual.h"

void cUIChat::open()
{
	ui.access.lock();
	int id;
	vec2f center(camera.res.x / 2.00f, camera.res.y / 2.00f);
	float resModX = (float)camera.res.x / 1280.00f;
	float resModY = (float)camera.res.y / 800.00f;

	ui.removeElementsByRef(REF_UI_CHAT);

	// Background
	id = ui.addElement("image", vec2f(200.00f, camera.res.y - 200.00f));
	ui.element[ui.getElementId(id)].size = sf::Vector2f(400.00f, 200.00f);
	ui.element[ui.getElementId(id)].texture = visual.addTexture("black.png");
	ui.element[ui.getElementId(id)].addRef(REF_UI_CHAT);
	ui.element[ui.getElementId(id)].button.action = "-";

	ui.access.unlock();
}

void cUIChat::close()
{
	ui.removeElementsByRef(REF_UI_CHAT, 0.50f);
}