
#pragma once
#include "define.h"
#include "util.h"
#include "refSystem.h"

class cSound
{
public:
	string name;
	float volume;
	float minDist;
	float maxDist;

	cSound() {
		name = "";
		minDist = 0.00f;
		maxDist = 300.00f;
	}
	cSound(int null) {
		cSound();
	}
	cSound(string data, float maxVolume = 100.00f, float minDistance = 0.00f, float maxDistance = 300.00f) {
		name = data;
		volume = maxVolume;
		minDist = minDistance;
		maxDist = maxDistance;
	}
	cSound(const char* data) {
		name = data;
		volume = 100.00f;
		minDist = 0.00f;
		maxDist = 300.00f;
	}
};

class cSoundQueue : public cReference
{
public:
	cSound data;
	int unitId;
	bool loop;

	cSoundQueue() {

	}
	cSoundQueue(cSound input, int inUnitId, bool repeat, int ref = -1) {
		data = input;
		unitId = inUnitId;
		loop = repeat;
		if (ref != -1) { addRef(ref); }
	}
};

class cMusic
{
public:
	string name;
};

class cAudio
{
public:
	cMutex access;

	sf::Sound sound[LIMIT_SOUND];
	sf::SoundBuffer soundBuffer[LIMIT_SOUND];
	cSoundQueue soundData[LIMIT_SOUND];

	vector<cSoundQueue> soundQueue;

	void playSound(cSound data);
	void playSound(cSoundQueue data);
	int getFreeSound();

	cAudio()
	{

	}
};

extern cAudio audio;
void audioMain();