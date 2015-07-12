
#include "main.h"
#include "logic.h"
#include "game.h"
#include "camera.h"
#include "weather.h"
#include "math.h"
#include "particle.h"

float myTimer = 0.00f;
void cGameLogic::updateParticles(int elapsedTime)
{
	vec2f bufferArea = vec2f(0.00f, 0.00f);
	float timevar = (float)elapsedTime / 1000;
	timevar *= core.timeModifier;

	// Spawning particles
	int id;
	myTimer += timevar;
	if (weather.current == WEATHER_SNOW_LIGHT && myTimer >= 0.05f)
	{
		myTimer = 0;
		for (int i = 0; i < math.rand(3, 7); i++)
		{
			vec2f spawnPoint = vec2f(camera.pos.x + math.randf(0.00, camera.res.x), camera.pos.y + math.randf(0.00f, camera.res.y));
			id = particle.addUnit("weather_snow", spawnPoint, math.randf(265.00f, 275.00f), math.randf(1.00f, 2.00f), 45.00f);
			particle.unit[id].movementSpeed = math.rand(50, 100);
			particle.unit[id].updateGenData();
			particle.unit[id].setFade(FADE_IN, 0.50f);
		}
	}

	// Updating the particle data
	float shadowAngleMod = 4.00f;
	float shadowScaleMod = 1.00f;
	float shadowScaleMin = 1.20f;
	float shadowAngle = 0.00f, shadowScale = 1.20f;

	float timeLocal = game.timeOfDay;
	shadowScale = abs(timeLocal - 12.00f) / 6.00f * shadowScaleMod + shadowScaleMin;
	shadowAngle = (timeLocal - 12.00f) * shadowAngleMod;

	cParticleUnit* unit;
	for (int i = 0; i < (int)particle.unit.size(); i++)
	{
		unit = &particle.unit[i];
		unit->pos -= unit->moveVector * timevar;
		// Always on screen
		if (unit->hasRef(REF_PARTICLE_ONSCREEN))
		{
			if (unit->pos.x < camera.pos.x) { unit->pos.x += camera.res.x; }
			else if (unit->pos.x > camera.pos.x + camera.res.x) { unit->pos.x -= camera.res.x; }
			else if (unit->pos.y < camera.pos.y) { unit->pos.y += camera.res.y; }
			else if (unit->pos.y > camera.pos.y + camera.res.y) { unit->pos.y -= camera.res.y; }
		}
		unit->updateShadowPos(shadowAngle, shadowScale);
		// Life timer
		if (unit->fadeType != FADE_OUT)
		{
			unit->lifetime -= timevar;
			if (unit->lifetime <= 0.00f) {
				unit->moveVector = vec2f(0.00f, 0.00f);
				unit->lifetime = 0.00f;
				unit->setFade(FADE_OUT, 3.00f);
			}
		}
		// Fade
		if (unit->fadeType != FADE_STOP)
		{
			if (unit->fadeType == FADE_IN) {
				unit->fadeVal += timevar;
				if (unit->fadeVal >= unit->fadeMax) { unit->setFade(FADE_STOP, 1.00f); }
			}
			else if (unit->fadeType == FADE_OUT) {
				unit->fadeVal -= timevar;
				if (unit->fadeVal <= 0.00f) {
					particle.removeUnit(i);
					i -= 1;
				}
			}
		}
	}
}