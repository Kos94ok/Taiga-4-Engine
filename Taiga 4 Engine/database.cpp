
#include "main.h"

void cDatabase::init()
{
	loadUnits();
	loadItems();
	loadUI();
	loadTextures();
	loadExternal();
}

void cDatabase::loadUnits()
{
	int i = 0;
	unit[i].type = "missingno";
	unit[i].size = vec2(20, 20);
	unit[i].addRef(REF_UNIT_NORENDER);
	i += 1;

	// Actual units
	unit[i].type = "player";
	unit[i].size = vec2(64, 64);
	unit[i].center = vec2(32, 52);
	unit[i].movementSpeed = 75;
	unit[i].animData[ANIM_IDLE].up.tex = visual.addTexture("player_idle_up.png");
	unit[i].animData[ANIM_IDLE].upDiag.tex = visual.addTexture("player_idle_diagup.png");
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("player_idle_side.png");
	unit[i].animData[ANIM_IDLE].downDiag.tex = visual.addTexture("player_idle_diagdown.png");
	unit[i].animData[ANIM_IDLE].down.tex = visual.addTexture("player_idle_down.png");
	unit[i].animData[ANIM_MOVE].up.frameCount = 4;
	unit[i].animData[ANIM_MOVE].up.frameDelay = 0.25f;
	unit[i].animData[ANIM_MOVE].up.tex = visual.addTexture("player_move_up.png");
	unit[i].animData[ANIM_MOVE].upDiag.frameCount = 4;
	unit[i].animData[ANIM_MOVE].upDiag.frameDelay = 0.25f;
	unit[i].animData[ANIM_MOVE].upDiag.tex = visual.addTexture("player_move_diagup.png");
	unit[i].animData[ANIM_MOVE].side.frameCount = 4;
	unit[i].animData[ANIM_MOVE].side.frameDelay = 0.25f;
	unit[i].animData[ANIM_MOVE].side.tex = visual.addTexture("player_move_side.png");
	unit[i].animData[ANIM_MOVE].downDiag.frameCount = 4;
	unit[i].animData[ANIM_MOVE].downDiag.frameDelay = 0.25f;
	unit[i].animData[ANIM_MOVE].downDiag.tex = visual.addTexture("player_move_diagdown.png");
	unit[i].animData[ANIM_MOVE].down.frameCount = 4;
	unit[i].animData[ANIM_MOVE].down.frameDelay = 0.25f;
	unit[i].animData[ANIM_MOVE].down.tex = visual.addTexture("player_move_down.png");
	unit[i].animData[ANIM_WORK].side.frameCount = 4;
	unit[i].animData[ANIM_WORK].side.frameDelay = 0.25f;
	unit[i].animData[ANIM_WORK].side.tex = visual.addTexture("player_work_side.png");
	unit[i].animData[ANIM_ATTACK].side.frameCount = 4;
	unit[i].animData[ANIM_ATTACK].side.frameDelay = 0.25f;
	unit[i].animData[ANIM_ATTACK].side.tex = visual.addTexture("player_attack_side.png");
	unit[i].animData[ANIM_PICKUP].side.frameCount = 4;
	unit[i].animData[ANIM_PICKUP].side.frameDelay = 0.25f;
	unit[i].animData[ANIM_PICKUP].side.tex = visual.addTexture("player_pickup_side.png");
	unit[i].light.power = 320.00f;
	unit[i].light.texture = visual.addTexture("light_whiteWeak.png");
	unit[i].container.itemLimit = 1000;
	unit[i].interactDistance = 20.00f;
	unit[i].collisionDistance = 20.00f;
	i += 1;

	// Trees
	unit[i].type = "tree_basic_a";
	unit[i].size = vec2(100, 170);
	unit[i].center = vec2(50, 165);
	unit[i].resource = 500.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tree_basic_a_idle.png");
	unit[i].interactDistance = 35.00f;
	unit[i].collisionDistance = 10.00f;
	unit[i].addRef(REF_UNIT_HARVESTABLE);
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "tree_basic_b";
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tree_basic_b_idle.png");
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "tree_basic_c";
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tree_basic_c_idle.png");
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "tree_basic_d";
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tree_basic_d_idle.png");
	i += 1;

	unit[i].type = "campfire_full";
	unit[i].size = vec2(40, 50);
	unit[i].center = vec2(20, 45);
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("campfire_idle.png");
	unit[i].animData[ANIM_IDLE].side.frameCount = 4;
	unit[i].animData[ANIM_IDLE].side.frameDelay = 0.20f;
	unit[i].collisionDistance = 20.00f;
	unit[i].light.power = 700.00f;
	unit[i].light.texture = visual.addTexture("light_orange.png");
	unit[i].light.flickerMod = 0.04f;
	unit[i].light.flickerTime = 0.25f;
	i += 1;

	// Other
	unit[i].type = "editor";
	unit[i].size = vec2(0, 0);
	unit[i].center = vec2(0, 0);
	unit[i].container.itemLimit = 1000;
	unit[i].addRef(REF_UNIT_NOSAVE);
	unit[i].addRef(REF_UNIT_NORENDER);
	i += 1;

	unit[i].type = "item_a";
	unit[i].size = vec2(30, 30);
	unit[i].center = unit[i].size / 2.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("itemWorld_idle.png");
	unit[i].interactDistance = 15.00f;
	unit[i].container.itemLimit = 1;
	unit[i].addRef(REF_UNIT_PICKUP);
	i += 1;

	unit[i].type = "proj_rifle";
	unit[i].size = vec2(10, 1);
	unit[i].center = unit[i].size / 2.00f;
	//unit[i].shadowOffset = vec2(30, -30);
	unit[i].flyingHeight = 20.00f;
	unit[i].animData[ANIM_MOVE].side.tex = visual.addTexture("missile_rifleNew.png");
	unit[i].movementSpeed = 3500.00f;
	unit[i].addRef(REF_UNIT_ROTATE);
	unit[i].addRef(REF_UNIT_MISSILE);
	i += 1;

	unit[i].type = "arena_wall";
	unit[i].size = vec2(20.00f, 20.00f);
	unit[i].center = unit[i].size / 2.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("red.png");
	unit[i].addRef(REF_UNIT_NOSHADOW);
	unit[i].addRef(REF_UNIT_ALWAYSVISIBLE);
	i += 1;
}

void cDatabase::loadItems()
{
	int i = 0;
	item[i].type = "missingno";
	item[i].setName("missingno");
	item[i].addRef(REF_ITEM_NOCRAFT);
	i += 1;

	// Test - Voodoo items
	item[i].type = "human_ear";
	item[i].setName("Human ear");
	item[i].addDescrLine("Why do you even have this thing?!");
	item[i].weight = 0.10f;
	i += 1;

	item[i].type = "human_arm";
	item[i].setName("Human arm");
	item[i].addDescrLine("Drop it! Drop it right now!!");
	item[i].weight = 0.50f;
	i += 1;

	item[i].type = "human_leg";
	item[i].setName("Human leg");
	item[i].addDescrLine("You are a terrible person...");
	item[i].weight = 0.10f;
	i += 1;

	item[i].type = "voodoo_doll";
	item[i].setName("Voodoo doll");
	item[i].addDescrLine("The hell are you going to do with that?!");
	item[i].weight = 1.50f;
	item[i].dismantle.add("human_leg", 1);
	item[i].dismantle.add("human_arm", 1);
	item[i].dismantle.add("human_ear", 5);
	i += 1;

	// Category test items
	item[i].type = "weapon_rifle";
	item[i].setName("Hunter Rifle");
	item[i].category = CATEGORY_WEAPONS;
	item[i].addRef(REF_ITEM_EQUIP);
	item[i].addRef(REF_ITEM_ACTIVE);
	item[i].addRef(REF_ITEM_RIFLE);
	i += 1;

	// Editor items
	item[i].type = "tree_basic_a";
	item[i].setName("Tree A");
	item[i].addRef(REF_ITEM_EDITOR);
	item[i].addRef(REF_ITEM_EQUIP);
	item[i].addRef(REF_ITEM_ACTIVE);
	i += 1;

	item[i] = item[i - 1];
	item[i].type = "tree_basic_b";
	item[i].setName("Tree B");
	i += 1;

	item[i] = item[i - 1];
	item[i].type = "tree_basic_c";
	item[i].setName("Tree C");
	i += 1;

	item[i] = item[i - 1];
	item[i].type = "tree_basic_d";
	item[i].setName("Tree D");
	i += 1;

	item[i] = item[i - 1];
	item[i].type = "campfire_full";
	item[i].setName("Campfire");
	i += 1;

	// More test items
	item[i].type = "test_resourceManipulator";
	item[i].setName("Resource Manipulator");
	item[i].category = CATEGORY_OTHER;
	item[i].addDescrLine("Can create the resource from nowhere!");
	item[i].addRef(REF_ITEM_NODROP);
	i += 1;
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
}

void cDatabase::loadExternal()
{
	// Not yet implemented
}

