
#include "main.h"
#include "database.h"
#include "visual.h"

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
	unit[i].rotateSpeed = 360.00f;
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
	unit[i].animData[ANIM_WORK] = unit[i].animData[ANIM_MOVE];
	//unit[i].animData[ANIM_WORK].side.frameCount = 4;
	//unit[i].animData[ANIM_WORK].side.frameDelay = 0.25f;
	//unit[i].animData[ANIM_WORK].side.tex = visual.addTexture("player_work_side.png");
	unit[i].animData[ANIM_ATTACK].side.frameCount = 4;
	unit[i].animData[ANIM_ATTACK].side.frameDelay = 0.25f;
	unit[i].animData[ANIM_ATTACK].side.tex = visual.addTexture("player_attack_side.png");
	unit[i].animData[ANIM_PICKUP].side.frameCount = 4;
	unit[i].animData[ANIM_PICKUP].side.frameDelay = 0.25f;
	unit[i].animData[ANIM_PICKUP].side.tex = visual.addTexture("player_pickup_side.png");
	unit[i].light.power = 320.00f;
	unit[i].light.texture = visual.addTexture("light_whiteWeak.png");
	unit[i].container.itemLimit = LIMIT_ITEMS;
	unit[i].interactDistance = 30.00f;
	unit[i].collisionDistance = 20.00f;
	unit[i].selectionOffset = vec2f(-1.50f, -7.00f);
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
	unit[i].health = 500.00f;
	unit[i].maxHealth = 500.00f;
	unit[i].resource = 275.00f;
	unit[i].rotateSpeed = 5760.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tree_basic_a_idle.png");
	unit[i].animData[ANIM_DEATH].side.frameCount = 4;
	unit[i].animData[ANIM_DEATH].side.frameDelay = 0.75f;
	unit[i].animData[ANIM_DEATH].side.tex = visual.addTexture("tree_basic_a_death.png");
	unit[i].interactDistance = 15.00f;
	unit[i].collisionDistance = 10.00f;
	unit[i].selectionOffset.x = -1.00f;
	unit[i].selectionOffset.y = -3.00f;
	unit[i].drop.add("material_wood", 1, 1.00f);
	unit[i].drop.add("material_wood", 1, 0.75f);
	unit[i].drop.add("material_wood", 1, 0.50f);
	unit[i].addRef(REF_UNIT_TREE);
	unit[i].addRef(REF_UNIT_HARVESTABLE);
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "tree_basic_b";
	unit[i].center = vec2(52, 170);
	unit[i].shadowOffset = vec2(0, -1);
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tree_basic_b_idle.png");
	unit[i].selectionOffset.x = 0.00f;
	unit[i].selectionOffset.y = -3.00f;
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "tree_basic_c";
	unit[i].shadowOffset = vec2(0, 0);
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tree_basic_c_idle.png");
	unit[i].selectionOffset.x = -3.00f;
	unit[i].selectionOffset.y = -8.00f;
	i += 1;

	unit[i] = unit[i - 1];
	unit[i].type = "tree_basic_d";
	unit[i].center = vec2(50, 160);
	unit[i].shadowOffset = vec2(0, -10);
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tree_basic_d_idle.png");
	unit[i].selectionOffset.x = 0.00f;
	unit[i].selectionOffset.y = 0.00f;
	i += 1;

	// Stones
	unit[i].type = "stone_basic_a";
	unit[i].size = vec2(70, 70);
	unit[i].center = vec2(35, 50);
	unit[i].shadowOffset = vec2(0, -5);
	unit[i].health = 500.00f;
	unit[i].maxHealth = 500.00f;
	unit[i].resource = 275.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("stone_basic_a_idle.png");
	unit[i].interactDistance = 35.00f;
	unit[i].collisionDistance = 25.00f;
	unit[i].selectionOffset.y = -10.00f;
	unit[i].drop.add("material_stone", 1, 1.00f);
	unit[i].drop.add("material_stone", 1, 0.75f);
	unit[i].drop.add("material_stone", 1, 0.50f);
	unit[i].drop.add("material_flint", 1, 0.50f, POWER_STONE);
	unit[i].drop.add("material_flint", 1, 0.40f, POWER_STONE);
	unit[i].drop.add("material_flint", 1, 0.30f, POWER_STONE);
	unit[i].drop.add("material_iron_raw", 1, 0.75f, POWER_FLINT);
	unit[i].drop.add("material_iron_raw", 1, 0.50f, POWER_FLINT);
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
	unit[i].interactDistance = 20.00f;
	unit[i].collisionDistance = 20.00f;
	unit[i].selectionOffset = vec2f(0.00f, -10.00f);
	unit[i].light.power = 700.00f;
	unit[i].light.texture = visual.addTexture("light_orange.png");
	unit[i].light.flickerMod = 0.04f;
	unit[i].light.flickerTime = 0.25f;
	unit[i].sound.idle = cSound("fire2.ogg", 50.00f, 100.00f, 700.00f);
	i += 1;

	// Tent
	unit[i].type = "tent_basic";
	unit[i].size = vec2(94, 84);
	unit[i].center = vec2(47, 70);
	unit[i].shadowOffset = vec2(0, -15);
	unit[i].health = 5000.00f;
	unit[i].maxHealth = 5000.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("tent_basic.png");
	unit[i].interactDistance = 42.00f;
	unit[i].collisionDistance = 35.00f;
	unit[i].selectionOffset = vec2f(2.50f, -13.00f);
	unit[i].addRef(REF_UNIT_PACK);
	unit[i].addRef(REF_UNIT_PACK_TENT);
	i += 1;

	// Dummy units
	unit[i].type = "dummy_flashlight";
	unit[i].size = vec2(0, 0);
	unit[i].health = 5000.00f;
	unit[i].maxHealth = 5000.00f;
	unit[i].rotateSpeed = 720.00f;
	unit[i].collisionDistance = 0.00f;
	unit[i].light.power = 500.00f;
	unit[i].light.texture = visual.addTexture("lightdir_white.png");
	unit[i].light.directional = true;
	unit[i].light.priority = 1;
	unit[i].addRef(REF_UNIT_NOSAVE);
	unit[i].addRef(REF_UNIT_NORENDER);
	unit[i].addRef(REF_UNIT_NOCOLLISION);
	i += 1;

	unit[i].type = "dummy_torch";
	unit[i].size = vec2(0, 0);
	unit[i].health = 5000.00f;
	unit[i].maxHealth = 5000.00f;
	unit[i].collisionDistance = 0.00f;
	unit[i].light.power = 300.00f;
	unit[i].light.texture = visual.addTexture("light_orange.png");
	unit[i].light.flickerMod = 0.04f;
	unit[i].light.flickerTime = 0.25f;
	unit[i].addRef(REF_UNIT_NOSAVE);
	unit[i].addRef(REF_UNIT_NORENDER);
	unit[i].addRef(REF_UNIT_NOCOLLISION);
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
	unit[i].selectionPriority = 3;
	unit[i].addRef(REF_UNIT_PICKUP);
	i += 1;

	unit[i].type = "proj_rifle";
	unit[i].size = vec2(10, 2);
	unit[i].center = unit[i].size / 2.00f;
	unit[i].flyingHeight = 20.00f;
	unit[i].animData[ANIM_MOVE].side.tex = visual.addTexture("missile_rifleNew.png");
	unit[i].rotateSpeed = 5760.00f;
	unit[i].movementSpeed = 1500.00f;
	unit[i].light.power = 25.00f;
	unit[i].light.texture = visual.addTexture("light_orange.png");
	unit[i].addRef(REF_UNIT_ROTATE);
	unit[i].addRef(REF_UNIT_MISSILE);
	unit[i].addRef(REF_UNIT_NOSELECTION);
	unit[i].addRef(REF_UNIT_DAMAGE_RIFLE);
	i += 1;

	unit[i].type = "arena_wall";
	unit[i].size = vec2(20.00f, 20.00f);
	unit[i].center = unit[i].size / 2.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("red.png");
	unit[i].addRef(REF_UNIT_NOSHADOW);
	unit[i].addRef(REF_UNIT_ALWAYSVISIBLE_BOT);
	i += 1;

	unit[i].type = "fogofwar";
	unit[i].size = vec2(500.00f, 500.00f);
	unit[i].center = unit[i].size / 2.00f;
	unit[i].animData[ANIM_IDLE].side.tex = visual.addTexture("grey.png");
	unit[i].addRef(REF_UNIT_NOSHADOW);
	unit[i].addRef(REF_UNIT_NOSELECTION);
	unit[i].addRef(REF_UNIT_ALWAYSVISIBLE_BOT);
	i += 1;
}