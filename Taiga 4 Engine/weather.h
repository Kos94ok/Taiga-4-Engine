
#include "precomp.h"

#define WEATHER_CLEAR			0
#define WEATHER_SNOW			1
#define WEATHER_RAIN			2
#define WEATHER_CLOUD			3

class cWeatherCloud
{
public:
	int tex;
	float moveSpeed;
	vec2f pos;
	vec2f size;
	vec2f moveVector;
	float moveAngle;

	void updateGenData();
};

class cWeather
{
public:
	int current;
	float power;
	float cloudDensity;

	vector<cWeatherCloud> cloud;
	vector<sf::Texture> cloudTexture;
	void createCloud(vec2f pos, vec2f size);
	void removeCloud(int id);

	//int changeType;
	//float changeTimer;
	//float changeTimerMax;

	void set(int id, float power);
	void changeTo(int id, float power, float time);
	void setClouds(float power);

	cWeather();
};

extern cWeather weather;