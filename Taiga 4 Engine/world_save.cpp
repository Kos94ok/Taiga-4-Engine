
#include "main.h"

void cWorld::saveChunk(vec2i pos)
{
	access.lock();
	access.unlock();
}

void cWorld::loadChunk(vec2i pos)
{
	access.lock();
	game.access.lock();
	int attachIndex = -1;
	vec2 anchor = getChunkCenter(pos);

	// Adding units
	vector<cUnitEntry> unitList = getChunkUnitList(pos);
	for (int i = 0; i < (int)unitList.size(); i++)
	{
		int id = game.addUnit(unitList[i].type, anchor + unitList[i].pos);
		game.unit[id].chunkPos = pos;
	}
	map[pos.x][pos.y].isLoaded = true;
	game.access.unlock();
	access.unlock();
}

void cWorld::unloadChunk(vec2i pos)
{
	access.lock();
	if (!map[pos.x][pos.y].isLoaded) { cout << "[ERROR] Chunk (" << pos.x << "; " << pos.y << ") is not loaded!"; return; }
	game.access.lock();
	for (int i = 0; i < game.unitCounter; i++)
	{
		if (game.unit[i].chunkPos == pos && !game.unit[i].hasRef(REF_UNIT_NOUNLOAD))
		{
			game.removeUnit(game.unit[i].globalId);
			i -= 1;
		}
	}
	map[pos.x][pos.y].isLoaded = false;
	game.access.unlock();
	access.unlock();
}

vector<cUnitEntry> cWorld::getChunkUnitList(vec2i pos)
{
	vector<cUnitEntry> retVal;
	char buffer[256];
	string buf;
	cUnitEntry entry;
	ifstream file;
	file.open("Savefiles//" + save.worldName + "//" + to_string(pos.x) + "-" + to_string(pos.y) + ".chunk");
	if (file.good())
	{
		// Skipping to unit lines
		do
		{
			file.getline(buffer, 256);
			buf = buffer;
		}
		while (!file.eof() && buf != "[Units]");
		// Reading units
		while (!file.eof())
		{
			// Type
			file.getline(buffer, 256);	buf = buffer;
			if (buf.length() > 0)
			{
				entry.type = buffer;
				// Pos X
				file.getline(buffer, 256);	buf = buffer;
				entry.pos.x = math.stringToInt(buffer);
				// Pos Y
				file.getline(buffer, 256);	buf = buffer;
				entry.pos.y = math.stringToInt(buffer);
				// Pusing to array
				retVal.push_back(entry);
			}
		}
		// Closing
		file.close();
	}
	return retVal;
}

vector<cUnitEntry> cWorld::getBlueprintUnitList(int index)
{
	vector<cUnitEntry> retVal;
	if (index == -1) { cout << "[ERROR] Invalid blueprint id!" << "\n"; return retVal; }
	char buffer[256];
	string buf;
	cUnitEntry entry;
	ifstream file;
	file.open("Data//Blueprints//" + blueprint[index].name);
	if (file.good())
	{
		// Skipping to unit lines
		do
		{
			file.getline(buffer, 256);
			buf = buffer;
		}
		while (!file.eof() && buf != "[Units]");
		// Reading units
		while (!file.eof())
		{
			// Type
			file.getline(buffer, 256);	buf = buffer;
			if (buf.length() > 0)
			{
				entry.type = buffer;
				// Pos X
				file.getline(buffer, 256);	buf = buffer;
				entry.pos.x = math.stringToInt(buffer);
				// Pos Y
				file.getline(buffer, 256);	buf = buffer;
				entry.pos.y = math.stringToInt(buffer);
				// Pusing to array
				retVal.push_back(entry);
			}
		}
		// Closing
		file.close();
	}
	return retVal;
}