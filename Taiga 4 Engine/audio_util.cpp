
#include "main.h"
#include "audio.h"
#include "database.h"
#include "settings.h"

int cAudio::getFreeSound()
{
	for (int i = 0; i < LIMIT_SOUND; i++)
	{
		if (sound[i].getStatus() == sf::Sound::Stopped) { return i; }
	}
	return -1;
}