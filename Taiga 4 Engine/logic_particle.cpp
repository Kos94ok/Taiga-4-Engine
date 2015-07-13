
#include "main.h"
#include "logic.h"
#include "game.h"
#include "camera.h"
#include "weather.h"
#include "math.h"
#include "particle.h"

float cloudTimer = 0.00f;
float weatherTimer = 0.00f;
void cGameLogic::updateParticles(int elapsedTime)
{
	int id;
	vec2f bufferArea = vec2f(2000.00f, 2000.00f), cloudPos, cloudSizeVec;
	float timevar = (float)elapsedTime / 1000;
	timevar *= core.timeModifier;

	sf::FloatRect camRect(camera.pos.x, camera.pos.y, camera.res.x, camera.res.y);
	sf::FloatRect cloudRect;

	// Spawning clouds
	cloudTimer += timevar;
	if (weather.cloudDensity > 0.00f)
	{
		int cloudCount = (int)weather.cloud.size();
		while (cloudCount < weather.cloudDensity / 100.00f)
		{
			cloudCount = (int)weather.cloud.size();
			cloudTimer = 0.00f;
			if (cloudCount < weather.cloudDensity / 100.00f && math.randf(0.00f, 1.00f) < 1.00f)
			{
				float cloudSize = math.randf(300.00f, 500.00f);
				do
				{
					//cloudPos.x = camera.pos.x + camera.res.x + cloudSize;
					//cloudPos.y = camera.pos.y + math.randf(0.00f, camera.res.y);
					cloudPos.x = math.randf(camera.pos.x - bufferArea.x, camera.pos.x + camera.res.x + bufferArea.x);
					cloudPos.y = math.randf(camera.pos.y - bufferArea.y, camera.pos.y + camera.res.y + bufferArea.y);
					cloudSizeVec.x = cloudSize;
					cloudSizeVec.y = cloudSize;
					cloudRect = sf::FloatRect(cloudPos - cloudSizeVec / 2.00f, cloudSizeVec);
				} while (cloudRect.intersects(camRect));
				weather.createCloud(cloudPos, cloudSizeVec);
			}
		}
	}

	// Updating the cloud data
	cWeatherCloud* cloud;
	mutex.renderClouds.lock();
	for (int i = 0; i < (int)weather.cloud.size(); i++)
	{
		cloud = &weather.cloud[i];
		cloud->pos += cloud->moveVector * timevar;
		if (weather.cloud[i].pos.x < camera.pos.x - bufferArea.x) { weather.removeCloud(i); }
		else if (weather.cloud[i].pos.x > camera.pos.x + camera.res.x + bufferArea.x) { weather.removeCloud(i); }
		else if (weather.cloud[i].pos.y < camera.pos.y - bufferArea.y) { weather.removeCloud(i); }
		else if (weather.cloud[i].pos.y > camera.pos.y + camera.res.y + bufferArea.y) { weather.removeCloud(i); }
	}
	mutex.renderClouds.unlock();

	// Spawning particles
	weatherTimer += timevar;
	if (weather.current == WEATHER_SNOW && weatherTimer >= 0.05f)
	{
		weatherTimer = 0;
		int count = math.round(weather.power / 1000.00f);
		for (int i = 0; i < math.rand(max(0, count - 5), count); i++)
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
				unit->setFade(FADE_OUT, 0.50f);
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