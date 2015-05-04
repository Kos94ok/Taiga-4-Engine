
#include "main.h"

vector<string> getBlueprintList()
{
	int filesFound = 0;
	vector<string> retArray;
	WIN32_FIND_DATAA dataStruct;
	HANDLE hFind;

	hFind = FindFirstFileA("Data//Blueprints//*", &dataStruct);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		if (strlen(dataStruct.cFileName) > 2) { retArray.push_back(dataStruct.cFileName); }
		while (FindNextFileA(hFind, &dataStruct))
		{
			if (strlen(dataStruct.cFileName) > 2) { retArray.push_back(dataStruct.cFileName); }
		}
	}
	return retArray;
}

void cWorld::analyzeBlueprints()
{
	blueprint.clear();
	bool printGood;
	int countCorrect = 0, countWrong = 0, value = -1;
	char buf[256];
	cBlueprint print;
	ifstream file;
	vector<string> list = getBlueprintList();
	for (int i = 0; i < (int)list.size(); i++)
	{
		print.name = list[i];
		// Opening the file
		file.open("Data//Blueprints//" + list[i]);
		if (file.good())
		{
			printGood = true;
			// Blueprint type
			file.getline(buf, 256);
			stringstream(buf) >> value;
			print.type = value;
			if (value < CHUNK_UNDEFINED || value > CHUNK_VILLAGE) { printGood = false; }
			// Closing the file
			file.close();
			// Pushing to array
			if (printGood)
			{
				blueprint.push_back(print);
				countCorrect += 1;
			}
			else { countWrong += 1; }
		}
		else { countWrong += 1; }
	}
	cout << "[MAIN] Blueprints analyzed: " << (int)list.size() << ". Correct: " << countCorrect << ", Wrong: " << countWrong << "\n";
}

void worldLoaderMain()
{
	int threadId = 7;
	cout << "[WORLD_LOADER] Starting the world loader thread" << "\n";

	sf::Packet data;
	bool isViable;
	while (!core.shutdown)
	{
		if (!core.editorMode)
		{
			for (int y = 0; y < LIMIT_MAP; y++)
			{
				for (int i = 0; i < LIMIT_MAP; i++)
				{
					//if (world.map[i][y].genStatus != CHUNK_UNDEFINED)
					//if (world.map[i][y].genStatus == CHUNK_NORMAL)
					//{
						isViable = world.isChunkViable(vec2i(i, y));
						// Loading
						if (isViable && !world.isChunkLoaded(vec2i(i, y)))
						{
							// Server-side routine
							if (core.localServer || core.serverMode)
							{
								world.loadChunk(vec2i(i, y));
							}
							// Client-side routine
							else if (client.connected)
							{
								world.map[i][y].isLoaded = true;
								// Politely ask the server for chunk (i; y)
								data << MSG_REQUEST_CHUNKDATA << i << y;
								client.sendPacket(data);
								data.clear();
							}
						}
						// Unloading
						else if (!isViable && world.isChunkLoaded(vec2i(i, y)))
						{
							// TODO: Fix server chunk unloading
							if (core.localServer || core.serverMode) { world.saveChunk(vec2i(i, y)); }
							world.unloadChunk(vec2i(i, y));
						}
					//}
				}
			}
		}
		Sleep(5);
		core.thread_antifreeze[threadId] = 0;
	}

	cout << "[WORLD_LOADER] Cleaning up" << "\n";
}