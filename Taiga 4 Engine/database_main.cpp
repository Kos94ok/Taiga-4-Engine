
#include "main.h"

void cDatabase::init()
{
	// Locking the access
	access.lock();
	// Clearing everything
	database.clear();
	// Loading assets
	console << "[DATABASE] Loading Items" << endl;
	loadItems();
	console << "[DATABASE] Loading Units" << endl;
	loadUnits();
	console << "[DATABASE] Loading UI" << endl;
	loadUI();
	console << "[DATABASE] Loading Textures" << endl;
	loadTextures();
	console << "[DATABASE] Loading External" << endl;
	loadExternal();
	console << "[DATABASE] Main database ready" << endl;
	// Releasing the lock 
	access.unlock();
}

void cDatabase::loadUI()
{
	int i = 0;
	uiElement[i].type = "missingno";
	uiElement[i].size = vec2(20, 20);
	i += 1;

	uiElement[i].type = "icon";
	uiElement[i].addRef(REF_UI_ICON);
	i += 1;

	uiElement[i].type = "button";
	uiElement[i].addRef(REF_UI_BUTTON);
	uiElement[i].size = vec2(128.00f, 32.00f);
	uiElement[i].texture = visual.addTexture("alpha.png");
	uiElement[i].textureHovered = visual.addTexture("ui_btn_hover.png");
	uiElement[i].button.action = "noAction";
	i += 1;

	uiElement[i].type = "text";
	uiElement[i].addRef(REF_UI_TEXT);
	uiElement[i].setText("");
	uiElement[i].size = vec2(0.00f, 0.00f);
	i += 1;

	uiElement[i].type = "image";
	uiElement[i].addRef(REF_UI_IMAGE);
	uiElement[i].textureHovered = -1;
	i += 1;


	uiElement[i].type = "icon_health";
	uiElement[i].size = vec2(64, 64);
	uiElement[i].tooltip.delay = 0.00f;
	uiElement[i].tooltip.setText("Health");
	uiElement[i].tooltip.offset.y = -20.00f;
	uiElement[i].texture = visual.addTexture("ui_icon_health.png");
	uiElement[i].bar.texture = visual.addTexture("ui_icon_health_full.png");
	uiElement[i].button.action = "-";
	uiElement[i].addRef(REF_UI_GAME);
	i += 1;

	uiElement[i] = database.getUIElement("icon_health");
	uiElement[i].type = "icon_mana";
	uiElement[i].size = vec2(64, 64);
	uiElement[i].tooltip.setText("Mana");
	i += 1;

	uiElement[i].type = "button_test";
	uiElement[i].setText("");
	uiElement[i].size = vec2(128, 32);
	uiElement[i].texture = visual.addTexture("ui_btn.png");
	uiElement[i].textureHovered = visual.addTexture("ui_btn_hover.png");
	uiElement[i].addRef(REF_UI_MAINMENU);
	uiElement[i].button.action = "button_test";
	uiElement[i].tooltip.delay = 0.50f;
	uiElement[i].tooltip.setText("");
	uiElement[i].tooltip.offset.y = -20.00f;
	i += 1;
}

void cDatabase::loadTextures()
{
	database.texture[TEX_UI_TOOLTIP] = visual.addTexture("ui_tooltip.png");
	database.texture[TEX_WORLD_GROUND] = visual.addTexture("bg_snow.jpg");
	database.texture[TEX_SELECTION_CIRCLE] = visual.addTexture("selectionWhite512.png");
}