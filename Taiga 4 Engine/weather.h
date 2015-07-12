
#include "precomp.h"

#define WEATHER_SUNNY			0
#define WEATHER_SNOW_LIGHT		1

class cWeather
{
public:
	int current;
	int changeType;
	float changeTimer;
	float changeTimerMax;

	void set(int id);
	void changeTo(int id, float time);

	cWeather() {
		current = WEATHER_SUNNY;
	}
};

extern cWeather weather;