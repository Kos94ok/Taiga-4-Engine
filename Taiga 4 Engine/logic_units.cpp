
#include "main.h"
#include "logic.h"
#include "game.h"
#include "math.h"
#include "settings.h"
#include "visual.h"
#include "window.h"

void cGameLogic::updateAnim(int elapsedTime)
{
	//game.access.lock();
	mutex.renderUnits.lock();
	float timevar = (float)elapsedTime / 1000;
	timevar *= core.timeModifier;
	// Unit animations
	for (int i = 0; i < game.unitCounter; i++)
	{
		cAnimDirection animDir = game.unit[i].animDisplay.data;
		// Sprite animation
		game.unit[i].anim.curFrameTimer += timevar;
		if (game.unit[i].anim.curFrameTimer >= animDir.frameDelay)
		{
			game.unit[i].anim.curFrameTimer = 0.00f;
			game.unit[i].anim.curFrame += 1;
			if (game.unit[i].anim.curFrame >= animDir.frameCount)
			{
				game.unit[i].anim.curFrame = 0;
			}
		}
		// Health bar animation
		if (game.unit[i].displayHealth != game.unit[i].health)
		{
			if (game.unit[i].displayHealth < game.unit[i].health)
			{
				game.unit[i].displayHealth += timevar * 100.00f;
				if (game.unit[i].displayHealth > game.unit[i].health) { game.unit[i].displayHealth = game.unit[i].health; }
			}
			else if (game.unit[i].displayHealth > game.unit[i].health)
			{
				game.unit[i].displayHealth -= timevar * 100.00f;
				if (game.unit[i].displayHealth < game.unit[i].health) { game.unit[i].displayHealth = game.unit[i].health; }
			}
		}
		// Light source animation
		if (game.unit[i].light.power > 0.00f && game.unit[i].light.flickerMod != 0.00f)
		{
			game.unit[i].light.flickerCurTime += timevar * math.randf(0.50f, 1.50f);
			if (game.unit[i].light.flickerCurTime >= game.unit[i].light.flickerTime * 2.00f)
			{
				game.unit[i].light.flickerCurTime -= game.unit[i].light.flickerTime * 2.00f;
			}
		}
	}
	mutex.renderUnits.unlock();
	//game.access.unlock();

	// World anim
	game.timeOfDay += timevar * settings.wdDayNightSpeed;
	if (game.timeOfDay >= 24.00f) { game.timeOfDay = 0.00f; }
	if (game.lightens() && game.ambientLight < game.ambientLightMax)
	{
		game.ambientLight += timevar * settings.wdNightChangeSpeed;
		if (game.ambientLight > game.ambientLightMax) { game.ambientLight = game.ambientLightMax; }
	}
	else if (game.darkens() && game.ambientLight > game.ambientLightMin)
	{
		game.ambientLight -= timevar * settings.wdNightChangeSpeed;
		if (game.ambientLight < game.ambientLightMin) { game.ambientLight = game.ambientLightMin; }
	}

	// Hovered unit
	mutex.renderMain.lock();
	int oldHover = visual.hoveredUnit;
	int oldPriority = -1;
	visual.hoveredUnit = -1;
	vec2f mousePos = window.getMousePos(true);
	for (int i = 0; i < game.unitCounter; i++)
	{
		if (!game.unit[i].hasRef(REF_UNIT_NOSELECTION)
			&& util.intersects(mousePos, game.unit[i].pos - game.unit[i].center, game.unit[i].size)
			&& (visual.hoveredUnit == -1 || game.unit[i].globalId == oldHover || game.unit[i].selectionPriority > oldPriority))
		{
			visual.hoveredUnit = game.unit[i].globalId;
			oldPriority = game.unit[i].selectionPriority;
		}
	}
	mutex.renderMain.unlock();
}

void cGameLogic::updateUnits(int elapsedTime)
{
	float timevar = (float)elapsedTime / 1000;
	timevar *= core.timeModifier;

	for (int i = 0; i < game.unitCounter; i++)
	{
		// Checking rotation
		if (abs(game.unit[i].facingAngle - game.unit[i].targetFacingAngle) > 0.50f) {
			float realAngle = game.unit[i].facingAngle;
			float targetAngle = game.unit[i].targetFacingAngle;
			//if (abs(targetAngle - realAngle) >) { realAngle += 360.00f; }
			//console.debug << "[DEBUG] Current: " << game.unit[i].facingAngle << ", Target: " << game.unit[i].targetFacingAngle << ", Speed: " << game.unit[i].rotateSpeed << endl;
			if (abs(realAngle - targetAngle) <= game.unit[i].rotateSpeed * timevar) {
				game.unit[i].facingAngle = game.unit[i].targetFacingAngle;
			}
			else {
				if (targetAngle - realAngle > 180.00f) { game.unit[i].facingAngle -= game.unit[i].rotateSpeed * timevar; }
				else if (realAngle < targetAngle || realAngle - targetAngle > 180.00f) { game.unit[i].facingAngle += game.unit[i].rotateSpeed * timevar; }
				else { game.unit[i].facingAngle -= game.unit[i].rotateSpeed * timevar; }
			}
			// No overshoot
			if (game.unit[i].facingAngle > 360.00f) { game.unit[i].facingAngle -= 360.00f; }
			else if (game.unit[i].facingAngle < 0.00f) { game.unit[i].facingAngle += 360.00f; }
		}
		// Checking buffs
		for (int y = 0; y < (int)game.unit[i].buff.list.size(); y++)
		{
			if (game.unit[i].buff.list[y].duration > 0.00f) {
				game.unit[i].buff.list[y].duration -= timevar;
				if (game.unit[i].buff.list[y].duration <= 0.00f) {
					game.unit[i].buff.removeById(y);
				}
			}
		}
		// Checking max health
		if (game.unit[i].health > game.unit[i].maxHealth) { game.unit[i].health = game.unit[i].maxHealth; }
		// Checking for death
		if (game.unit[i].maxHealth > 0.00 && game.unit[i].health <= 0.00f && game.unit[i].order[0].type != ORDER_DEATH)
		{
			game.killUnit(game.unit[i].globalId);
		}
		// Checking life timer
		if (game.unit[i].lifeTimer.enabled)
		{
			game.unit[i].lifeTimer.time -= timevar;
			// Your time has come...
			if (game.unit[i].lifeTimer.time <= 0.00f)
			{
				if (game.unit[i].globalId == ID_LOCAL) { game.removeLocalUnit(i); }
				else { game.removeUnit(game.unit[i].globalId); }
				i -= 1;
			}
		}
		// Update animation data
		//game.unit[i].updateDisplayAnim();
	}
}