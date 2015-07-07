
#include "UI.h"
#include "chat.h"
#include "camera.h"
#include "visual.h"
#include "settings.h"

void cUIChat::open()
{
	ui.access.lock();
	int id;
	vec2f center(camera.res.x / 2.00f, camera.res.y / 2.00f);
	float resModX = (float)camera.res.x / 1280.00f;
	float resModY = (float)camera.res.y / 800.00f;

	ui.removeElementsByRef(REF_UI_CHAT);

	vec2f chatPos = chat.getPos();
	vec2f chatSize = chat.getSize();

	// Background
	id = ui.addElement("image", chatPos + chatSize / 2.00f);
	ui.element[ui.getElementId(id)].size = chatSize;
	ui.element[ui.getElementId(id)].texture = visual.addTexture("black.png");
	ui.element[ui.getElementId(id)].addRef(REF_UI_CHAT);
	//ui.element[ui.getElementId(id)].button.action = "-";
	ui.element[ui.getElementId(id)].priority = 2;
	if (!chat.inFocus) {
		ui.element[ui.getElementId(id)].alpha = 125.00f;
	}
	else {
		ui.element[ui.getElementId(id)].alpha = 200.00f;
	}
	// Border
	ui.createBorder(chatPos, chatPos + chatSize, REF_UI_CHAT, 3);
	// Scroll buttons
	if (chat.inFocus)
	{
		id = ui.addElement("button", chatPos + vec2f(chatSize.x - 14.00f, 14.00f));
		ui.element[ui.getElementId(id)].size = vec2f(24.00, 24.00f);
		ui.element[ui.getElementId(id)].texture = visual.addTexture("ui_icon_up_white.png");
		ui.element[ui.getElementId(id)].textureHovered = visual.addTexture("ui_icon_up_orange.png");
		ui.element[ui.getElementId(id)].button.action = "chat_scrollUp";
		ui.element[ui.getElementId(id)].addRef(REF_UI_CHAT);
		ui.element[ui.getElementId(id)].priority = 3;
		id = ui.addElement("button", chatPos + vec2f(chatSize.x - 14.00f, 40.00f));
		ui.element[ui.getElementId(id)].size = vec2f(24.00, 24.00f);
		ui.element[ui.getElementId(id)].texture = visual.addTexture("ui_icon_down_white.png");
		ui.element[ui.getElementId(id)].textureHovered = visual.addTexture("ui_icon_down_orange.png");
		ui.element[ui.getElementId(id)].button.action = "chat_scrollDown";
		ui.element[ui.getElementId(id)].addRef(REF_UI_CHAT);
		ui.element[ui.getElementId(id)].priority = 3;
	}

	update();
	ui.setFadeTimeByRef(REF_UI_CHAT, 0.10f, FADE_IN);

	ui.access.unlock();
}

void cUIChat::close()
{
	if (chat.inFocus) { ui.removeElementsByRef(REF_UI_CHAT, 0.10f); }
	else { ui.removeElementsByRef(REF_UI_CHAT, 0.10f); }
}

void cUIChat::update()
{
	ui.access.lock();
	int id;
	float resModX = (float)camera.res.x / 1280.00f;
	float resModY = (float)camera.res.y / 800.00f;

	ui.removeElementsByRef(REF_UI_CHAT_TEXT);

	vec2f chatPos = chat.getPos();
	vec2f chatSize = chat.getSize();

	// Text
	for (int i = max(0, (int)chat.history[chat.displayedPage].size() - chat.getLineCount() - chat.scrollOffset); i < (int)chat.history[chat.displayedPage].size() - max(0, chat.scrollOffset); i++)
	{
		id = ui.createText(vec2f(3.00f, camera.res.y - 400.00f + i * 17.00f), chat.history[chat.displayedPage][i], "", REF_UI_CHAT);
		ui.element[ui.getElementId(id)].pos = vec2f(chatPos.x + 7.00f, (i - max(0, (int)chat.history[chat.displayedPage].size() - chat.getLineCount()) + chat.scrollOffset) *
			(settings.chatFontSize + settings.chatLineSpacing) + chatPos.y + 3.00f);
		ui.element[ui.getElementId(id)].textFont = FONT_MAIN;
		ui.element[ui.getElementId(id)].textSize = settings.chatFontSize;
		//ui.element[ui.getElementId(id)].size = sf::Vector2f(400.00f, 200.00f);
		//ui.element[ui.getElementId(id)].texture = visual.addTexture("black.png");
		ui.element[ui.getElementId(id)].addRef(REF_UI_CHAT_TEXT);
		ui.element[ui.getElementId(id)].button.action = "-";
		ui.element[ui.getElementId(id)].priority = 4;
	}

	ui.access.unlock();
}