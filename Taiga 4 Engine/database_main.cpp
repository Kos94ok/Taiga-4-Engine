
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
	unit[i].health = 100.00f;
	unit[i].maxHealth = 100.00f;
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
	unit[i].addRef(REF_UNIT_NOUNLOAD);
	unit[i].addRef(REF_UNIT_BESTPATHING);
	i += 1;

	unit[i].type = "enemy";
	unit[i].size = vec2(64, 64);
	unit[i].center = vec2(32, 32);
	unit[i].health = 10.00f;
	unit[i].maxHealth = 10.00f;
	unit[i].movementSpeed = 80;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("enemy_idle_side.png");
	unit[i].animData[ANIM_MOVE].side.tex = visual.addTexture("enemy_idle_side.png");
	unit[i].light.power = 100.00f;
	unit[i].light.texture = visual.addTexture("light_orange.png");
	unit[i].interactDistance = 20.00f;
	unit[i].collisionDistance = 20.00f;
	unit[i].ai = cAI::think_enemy;
	unit[i].addRef(REF_UNIT_NOUNLOAD);
	unit[i].addRef(REF_UNIT_BESTPATHING);
	i += 1;

	// Trees
	unit[i].type = "tree_basic_a";
	unit[i].size = vec2(100, 170);
	unit[i].center = vec2(50, 167);
	unit[i].health = 5000.00f;
	unit[i].maxHealth = 5000.00f;
	unit[i].resource = 500.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tree_basic_a_idle.png");
	unit[i].animData[ANIM_DEATH].side.frameCount = 4;
	unit[i].animData[ANIM_DEATH].side.frameDelay = 0.75f;
	unit[i].animData[ANIM_DEATH].side.tex = visual.addTexture("tree_basic_a_death.png");
	unit[i].interactDistance = 35.00f;
	unit[i].collisionDistance = 10.00f;
	unit[i].addItem("material_wood");
	unit[i].addRef(REF_UNIT_TREE);
	unit[i].addRef(REF_UNIT_HARVESTABLE);
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "tree_basic_b";
	unit[i].center = vec2(52, 170);
	unit[i].shadowOffset = vec2(0, -1);
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tree_basic_b_idle.png");
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "tree_basic_c";
	unit[i].shadowOffset = vec2(0, 0);
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tree_basic_c_idle.png");
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "tree_basic_d";
	unit[i].center = vec2(50, 160);
	unit[i].shadowOffset = vec2(0, -10);
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tree_basic_d_idle.png");
	i += 1;

	// Stones
	unit[i].type = "stone_basic_a";
	unit[i].size = vec2(70, 70);
	unit[i].center = vec2(35, 50);
	unit[i].shadowOffset = vec2(0, -5);
	unit[i].health = 5000.00f;
	unit[i].maxHealth = 5000.00f;
	unit[i].resource = 500.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("stone_basic_a_idle.png");
	unit[i].interactDistance = 35.00f;
	unit[i].collisionDistance = 25.00f;
	unit[i].addItem("material_stone");
	unit[i].addRef(REF_UNIT_STONE);
	unit[i].addRef(REF_UNIT_HARVESTABLE);
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "stone_basic_b";
	unit[i].center = vec2(35, 60);
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("stone_basic_b_idle.png");
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "stone_basic_c";
	unit[i].center = vec2(35, 55);
	unit[i].shadowOffset = vec2(0, -5);
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("stone_basic_c_idle.png");
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "stone_basic_d";
	unit[i].center = vec2(35, 55);
	unit[i].shadowOffset = vec2(0, -15);
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("stone_basic_d_idle.png");
	i += 1;

	// Campfire
	unit[i].type = "campfire_full";
	unit[i].size = vec2(40, 50);
	unit[i].center = vec2(20, 45);
	unit[i].health = 5000.00f;
	unit[i].maxHealth = 5000.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("campfire_idle.png");
	unit[i].animData[ANIM_IDLE].side.texShadow = visual.addTexture("campfire_idle_shadow.png");
	unit[i].animData[ANIM_IDLE].side.frameCount = 4;
	unit[i].animData[ANIM_IDLE].side.frameDelay = 0.20f;
	unit[i].collisionDistance = 20.00f;
	unit[i].light.power = 700.00f;
	unit[i].light.texture = visual.addTexture("light_orange.png");
	unit[i].light.flickerMod = 0.04f;
	unit[i].light.flickerTime = 0.25f;
	i += 1;

	// Tent
	unit[i].type = "tent_basic";
	unit[i].size = vec2(120, 100);
	unit[i].center = vec2(60, 70);
	unit[i].shadowOffset = vec2(0, -15);
	unit[i].health = 5000.00f;
	unit[i].maxHealth = 5000.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tent_basic.png");
	unit[i].collisionDistance = 35.00f;
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
	unit[i].light.power = 100.00f;
	unit[i].light.texture = visual.addTexture("light_blueWeak.png");
	unit[i].addRef(REF_UNIT_PICKUP);
	i += 1;

	unit[i].type = "proj_rifle";
	unit[i].size = vec2(10, 2);
	unit[i].center = unit[i].size / 2.00f;
	unit[i].flyingHeight = 20.00f;
	unit[i].animData[ANIM_MOVE].side.tex = visual.addTexture("missile_rifleNew.png");
	unit[i].movementSpeed = 1500.00f;
	unit[i].light.power = 25.00f;
	unit[i].light.texture = visual.addTexture("light_orange.png");
	unit[i].addRef(REF_UNIT_ROTATE);
	unit[i].addRef(REF_UNIT_MISSILE);
	unit[i].addRef(REF_UNIT_DAMAGE_RIFLE);
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

	// Weapons
		// Hunting rifle
	item[i].type = "weapon_rifle";
	item[i].setName("Hunting Rifle");
	item[i].addDescrLine("Этот текст все равно потом поменяется.");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Equippable");
	item[i].category = CATEGORY_WEAPONS;
	item[i].addRef(REF_ITEM_EQUIP);
	item[i].addRef(REF_ITEM_ACTIVE);
	item[i].addRef(REF_ITEM_RIFLE);
	item[i].icon = visual.createIcon("icons_a.png", "icon_rifle", vec2i(7, 0), vec2f(0.125f, 0.125f));
	i += 1;

		// Steel knife
	item[i].type = "weapon_knife_steel";
	item[i].setName("Steel Knife");
	item[i].addDescrLine("Лень что-то писать :(");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Equippable");
	item[i].category = CATEGORY_WEAPONS;
	item[i].addRef(REF_ITEM_EQUIP);
	i += 1;

	// Tools
		// Axe
	item[i].type = "tool_axe_stone";
	item[i].setName("Stone Axe");
	item[i].addDescrLine("Used to chop some wood.");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Equippable");
	item[i].category = CATEGORY_WEAPONS;
	item[i].addRef(REF_ITEM_EQUIP);
	item[i].addRef(REF_ITEM_ACTIVE);
	item[i].addRef(REF_ITEM_AXE);
	i += 1;

		// Pickaxe
	item[i].type = "tool_pickaxe_stone";
	item[i].setName("Stone Pickaxe");
	item[i].addDescrLine("Used to pick some stone.");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Equippable");
	item[i].category = CATEGORY_WEAPONS;
	item[i].addRef(REF_ITEM_EQUIP);
	item[i].addRef(REF_ITEM_ACTIVE);
	item[i].addRef(REF_ITEM_PICKAXE);
	item[i].icon = visual.createIcon("icons_b.png", "icon_pickaxe", vec2i(0, 2), vec2f(0.125f, 0.125f));
	i += 1;

	// Food
		// Canned meat
	item[i].type = "food_meat_canned";
	item[i].setName("Canned Meat");
	item[i].addDescrLine("Some meat in the can. Good to win some time");
	item[i].addDescrLine("against hunger.");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Restores 25 health");
	item[i].addDescrLine("- Restores Y hunger");
	item[i].addDescrLine("- Consumable");
	item[i].dismantle.add("material_tin", 1);
	item[i].consume.healthBalance = 25;
	item[i].addRef(REF_ITEM_CONSUME);
	item[i].addRef(REF_ITEM_EMPTIABLE);
	item[i].addRef(REF_ITEM_DISMANTLE_ON_CONSUME);
	item[i].category = CATEGORY_CONSUMABLES;
	i += 1;

		// Bottle of water
	item[i].type = "food_water_bottle";
	item[i].setName("Bottle of Water");
	item[i].addDescrLine("A bottle full of fresh water. Restores some");
	item[i].addDescrLine("energy and gives more strength to survive.");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Restores 50 health");
	item[i].addDescrLine("- Consumable");
	item[i].consume.healthBalance = 50;
	item[i].addRef(REF_ITEM_CONSUME);
	item[i].category = CATEGORY_CONSUMABLES;
	i += 1;

	// Materials
		// Flint
	item[i].type = "flint_basic";
	item[i].setName("Flint");
	item[i].addDescrLine("A simple but powerful tool to start a fire in");
	item[i].addDescrLine("the wilderness. Combine with the dagger to get");
	item[i].addDescrLine("some sparks.");
	item[i].category = CATEGORY_MATERIALS;
	i += 1;

		// Tin
	item[i].type = "material_tin";
	item[i].setName("Tin");
	item[i].addDescrLine("A small piece of tin. Can be useful.");
	item[i].category = CATEGORY_MATERIALS;
	i += 1;

		// Wood
	item[i].type = "material_wood";
	item[i].setName("Wood");
	item[i].addDescrLine("A wooden log.");
	item[i].category = CATEGORY_MATERIALS;
	i += 1;

		// Stone
	item[i].type = "material_stone";
	item[i].setName("Stone");
	item[i].addDescrLine("A big stone.");
	item[i].category = CATEGORY_MATERIALS;
	i += 1;

	// Other
		// Basic survival kit
	item[i].type = "survival_kit_basic";
	item[i].setName("Basic Survival Kit");
	item[i].addDescrLine("Contains some essential survival equipment.");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Unpackable");
	item[i].dismantle.add("weapon_knife_steel", 1);
	item[i].dismantle.add("food_meat_canned", 5);
	item[i].dismantle.add("food_water_bottle", 1);
	item[i].dismantle.add("flint_basic", 3);
	item[i].addRef(REF_ITEM_UNPACKABLE);
	item[i].category = CATEGORY_OTHER;
	i += 1;

		// Campfire
	item[i].type = "campfire_basic";
	item[i].setName("Campfire");
	item[i].addDescrLine("Incredibly valuable item during the long and cold");
	item[i].addDescrLine("night in the wilds.");
	item[i].addDescrLine("");
	item[i].addDescrLine("- Buildable");
	item[i].category = CATEGORY_OTHER;
	i += 1;

	// Regular Shooter - Story Mode
	item[i].type = "human_eye";
	item[i].setName("Human eye");
	item[i].addDescrLine("It looks inside you...");
	item[i].addDescrLine("Оно смотрит прямо в тебя...");
	item[i].weight = 0.10f;
	i += 1;

	item[i].type = "human_ear";
	item[i].setName("Human ear");
	item[i].addDescrLine("Why do you even have this thing?!");
	item[i].addDescrLine("Почему ЭТО у тебя в руках?!");
	item[i].weight = 0.10f;
	i += 1;

	item[i].type = "human_arm";
	item[i].setName("Human arm");
	item[i].addDescrLine("Drop it! Drop it right now!!");
	item[i].addDescrLine("Брось! Брось бяку быстро!");
	item[i].weight = 5.00f;
	i += 1;

	item[i].type = "human_leg";
	item[i].setName("Human leg");
	item[i].addDescrLine("You are a terrible person...");
	item[i].addDescrLine("Ты - ужасный человек...");
	item[i].weight = 5.00f;
	i += 1;

	item[i].type = "voodoo_recipe";
	item[i].setName("Voodoo Recipe");
	item[i].addDescrLine("It says \"Use eyes, ears, arms and legs to");
	item[i].addDescrLine("create a really powerful vooo-dooo doll\".");
	item[i].addDescrLine("Тут написано \"Используй глаза, уши, руки");
	item[i].addDescrLine("и ноги, чтобы сделать действительно крутую");
	item[i].addDescrLine("куклу вуу-дуу.\".");
	i += 1;

	item[i].type = "voodoo_doll";
	item[i].setName("Voodoo doll");
	item[i].addDescrLine("You can use it to summon some slender guys...");
	item[i].addDescrLine("Используй эту штуку, чтобы вызвать что-нибудь");
	item[i].addDescrLine("слендероподобное...");
	item[i].weight = 1.50f;
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
	database.texture[TEX_ICON_MAIN] = visual.addTexture("icon_main.ico");
}

