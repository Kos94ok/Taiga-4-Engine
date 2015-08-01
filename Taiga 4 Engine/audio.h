
#pragma once
#include "define.h"
#include "util.h"
#include "refSystem.h"

#define AUDIO_MUSIC					0
#define AUDIO_EFFECT				1
#define AUDIO_AMBIENT				2
#define AUDIO_WEATHER				3
#define AUDIO_FOOTSTEPS				4

class cSound
{
public:
	string name;
	float volume;
	float minDist;
	float maxDist;
	int classification;

	cSound() {
		name = "";
		minDist = 0.00f;
		maxDist = 300.00f;
	}
	cSound(int null) {
		cSound();
	}
	cSound(string data, float maxVolume = 100.00f, float minDistance = 0.00f, float maxDistance = 300.00f, int soundType = AUDIO_EFFECT) {
		name = data;
		volume = maxVolume;
		minDist = minDistance;
		maxDist = maxDistance;
		classification = soundType;
	}
	cSound(const char* data) {
		name = data;
		volume = 100.00f;
		minDist = 0.00f;
		maxDist = 300.00f;
	}
};

class cSoundFootsteps
{
public:
	vector<cSound> data;
	vector<int> frames;

	void setFrames(int a = -1, int b = -1, int c = -1, int d = -1, int e = -1, int f = -1) {
		frames.clear();
		if (a != -1) { frames.push_back(a); }
		if (b != -1) { frames.push_back(b); }
		if (c != -1) { frames.push_back(c); }
		if (d != -1) { frames.push_back(d); }
		if (e != -1) { frames.push_back(e); }
		if (f != -1) { frames.push_back(f); }
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