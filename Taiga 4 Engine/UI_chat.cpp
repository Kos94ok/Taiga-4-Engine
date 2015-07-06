
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
	id = ui.addElement("image", vec2f(300.00f, camera.res.y - 200.00f));
	ui.element[ui.getElementId(id)].size = sf::Vector2f(600.00f, 400.00f);
	ui.element[ui.getElementId(id)].texture = visual.addTexture("black.png");
	ui.element[ui.getElementId(id)].addRef(REF_UI_CHAT);
	//ui.element[ui.getElementId(id)].button.action = "-";
	ui.element[ui.getElementId(id)].priority = 4;
	if (!chat.inFocus) {
		ui.element[ui.getElementId(id)].alpha = 125.00f;
	}
	else {
		ui.element[ui.getElementId(id)].alpha = 200.00f;
	}
	update();

	ui.access.unlock();
}

void cUIChat::close()
{
	if (chat.inFocus) { ui.removeElementsByRef(REF_UI_CHAT, 0.20f); }
	else { ui.removeElementsByRef(REF_UI_CHAT, 1.00f); }
}

void cUIChat::update()
{
	ui.access.lock();
	int id;
	float resModX = (float)camera.res.x / 1280.00f;
	float resModY = (float)camera.res.y / 800.00f;

	ui.removeElementsByRef(REF_UI_CHAT_TEXT);

	// Text
	for (int i = 0; i < (int)chat.history[chat.displayedPage].size(); i++)
	{
		id = ui.createText(vec2f(3.00f, camera.res.y - 400.00f + i * 17.00f), chat.history[chat.displayedPage][i], "", REF_UI_CHAT);
		ui.element[ui.getElementId(id)].textFont = FONT_MAIN;
		ui.element[ui.getElementId(id)].textSize = 16;
		//ui.element[ui.getElementId(id)].size = sf::Vector2f(400.00f, 200.00f);
		//ui.element[ui.getElementId(id)].texture = visual.addTexture("black.png");
		ui.element[ui.getElementId(id)].addRef(REF_UI_CHAT_TEXT);
		ui.element[ui.getElementId(id)].button.action = "-";
	}

	ui.access.unlock();
}