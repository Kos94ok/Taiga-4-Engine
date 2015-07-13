
#include "weather.h"
#include "math.h"
#include "camera.h"
#include "console.h"
#include "visual.h"
#include "database.h"

void cWeatherCloud::updateGenData()
{
	moveVector = vec2f(moveSpeed * cos(moveAngle * math.DEGTORAD), moveSpeed * sin(moveAngle * math.DEGTORAD));
}

void cWeather::set(int id, float power)
{
	current = id;
	this->power = power;
}

void cWeather::changeTo(int id, float power, float time)
{
	set(id, power);
	//changeType = id;
}

void cWeather::setClouds(float power)
{
	cloudDensity = power;
}

cWeatherCloud newCloud;
void cWeather::createCloud(vec2f pos, vec2f size)
{
	newCloud.tex = database.texture[TEX_GEN_CLOUD + math.rand(0, LIMIT_GENTEXCOUNT_CLOUD - 1)];
	newCloud.pos = pos;
	newCloud.size = size;
	newCloud.moveSpeed = math.randf(10.00f, 50.00f);
	newCloud.moveAngle = math.randf(170.00f, 190.00f);
	//sf::Texture gen = visual.genCloudTexture(size, pieceSizeMin, pieceSizeMax, pieceCount);
	newCloud.updateGenData();

	mutex.renderClouds.lock();
	cloud.push_back(newCloud);
	//cloudTexture.push_back(gen);
	mutex.renderClouds.unlock();
}

void cWeather::removeCloud(int id)
{
	cloud[id] = cloud[(int)cloud.size() - 1];
	cloud.pop_back();
	//cloudTexture[id] = cloudTexture[(int)cloudTexture.size() - 1];
	//cloudTexture.pop_back();
	//cloud.erase(cloud.begin() + id);
	//cloudTexture.erase(cloudTexture.begin() + id);
}

cWeather::cWeather() {
	current = WEATHER_CLEAR;
}