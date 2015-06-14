
void audioMain();

class cSound
{
public:
	std::string name;
	float fade;
	float volume;
	float minDist;
	float maxDist;

	cSound() {
		name = "";
		fade = 1.00f;
		minDist = 0.00f;
		maxDist = 300.00f;
	}
	cSound(std::string data, float maxVolume = 100.00f, float atten = 1.00f, float minDistance = 0.00f, float maxDistance = 300.00f) {
		name = data;
		fade = atten;
		volume = maxVolume;
		minDist = minDistance;
		maxDist = maxDistance;
	}
	cSound(const char* data) {
		name = data;
		fade = 1.00f;
		volume = 100.00f;
		minDist = 0.00f;
		maxDist = 300.00f;
	}
};

class cSoundQueue
{
public:
	cSound data;
	int unitId;
	bool loop;

	cSoundQueue() {

	}
	cSoundQueue(cSound input, int inUnitId, bool repeat) {
		data = input;
		unitId = inUnitId;
		loop = repeat;
	}
};

class cMusic
{
public:
	std::string name;
};

class cAudio
{
public:
	cMutex access;

	sf::Sound sound[LIMIT_SOUND];
	sf::SoundBuffer soundBuffer[LIMIT_SOUND];
	cSoundQueue soundData[LIMIT_SOUND];

	std::vector<cSoundQueue> soundQueue;

	void playSound(cSound data);
	void playSound(cSoundQueue data);
	int getFreeSound();

	cAudio()
	{

	}
};