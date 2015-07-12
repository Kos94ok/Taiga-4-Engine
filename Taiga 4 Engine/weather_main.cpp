
#include "weather.h"

void cWeather::set(int id)
{
	current = id;
}

void cWeather::changeTo(int id, float time)
{
	set(id);
	//changeType = id;
}