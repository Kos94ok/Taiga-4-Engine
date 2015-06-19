
#include "main.h"

void cAudio::playSound(cSound data)
{
	if (core.serverMode) { return; }
	soundQueue.push_back(cSoundQueue(data, -1, false));
}

void cAudio::playSound(cSoundQueue data)
{
	if (core.serverMode) { return; }
	soundQueue.push_back(data);
}

void audioMain()
{
	int threadId = 9;
	if (core.serverMode) { return; }
	console << "[AUDIO] Starting the audio thread" << "\n";
	int slot;
	cSound data;
	cUnit* unit;
	vec2f listener;
	cSoundQueue dataQueue;
	float dist;
	while (!core.thread_shutdown[threadId])
	{
		// Adjusting listener's position
		if (settings.enableCameraListener) {
			listener = camera.pos + vec2f(camera.res.x, camera.res.y) / 2.00f;
		}
		else if (client.unit != -1) {
			listener = game.getUnit(client.unit).pos;
		}
		// Playing the first sound from the queue
		if (audio.soundQueue.size() > 0 && audio.getFreeSound() != -1)
		{
			slot = audio.getFreeSound();
			// Getting the next sound in the queue
			data = audio.soundQueue[0].data;
			// Executing
			if (ifstream("Data//Sounds//" + data.name).good()) {
				// Adding additional information
				audio.soundData[slot] = audio.soundQueue[0];
				// Playing the sound
				audio.soundBuffer[slot].loadFromFile("Data//Sounds//" + data.name);
				audio.sound[slot].setBuffer(audio.soundBuffer[slot]);
				// Checking the volume
				if (audio.soundData[slot].unitId == -1) {
					audio.sound[slot].setVolume(data.volume);
				}
				else {
					audio.sound[slot].setVolume(0.00f);
				}
				audio.sound[slot].setLoop(audio.soundData[slot].loop);
				audio.sound[slot].play();
			}
			// Sound not found
			else {
				console.error << "[AUDIO] Can't open sound \"Data//Sounds//" + data.name + "\"" << endl;
			}
			// Erasing the used data
			audio.soundQueue.erase(audio.soundQueue.begin());
		}
		// No free sound slots
		else if (audio.soundQueue.size() > 0) {
			console.error << "[AUDIO] No free sound queue slots!" << endl;
		}
		// Checking the existing sounds
		for (int i = 0; i < LIMIT_SOUND; i++)
		{
			if (audio.sound[i].getStatus() == sf::Sound::Playing)
			{
				// Checking unit attachment
				if (audio.soundData[i].unitId != -1)
				{
					unit = &game.getUnit(audio.soundData[i].unitId);
					// Checking the volume if the unit is valid
					if (unit->type != "missingno") {
						dataQueue = audio.soundData[i];
						data = dataQueue.data;
						dist = math.getDistance(listener, unit->pos);

						if (dist < data.minDist) { audio.sound[i].setVolume(audio.soundData[i].data.volume * settings.volEffects * settings.volMaster); }
						else {
							audio.sound[i].setVolume(audio.soundData[i].data.volume * settings.volEffects * settings.volMaster *
								min(1.00f, max(0.00f, (data.maxDist - dist + data.minDist) / data.maxDist)) );
						}
					}
					// Stopping if the unit does not exist
					else if (unit->type == "missingno") {
						audio.sound[i].stop();
					}
				}
			}
		}
		// Thread routine
		core.thread_antifreeze[threadId] = 0;
		Sleep(1);
	}
	console << "[AUDIO] Cleaning up" << "\n";
	// Stopping all the sounds
	for (int i = 0; i < LIMIT_SOUND; i++)
	{
		audio.sound[i].stop();
		audio.sound[i].resetBuffer();
	}
}