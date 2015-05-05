
#include "main.h"

void cWorld::saveChunk(vec2i pos)
{
	/*
	WARNING:
	This function is only to be used on the server side!
	Client does not store any world data on the disk.
	*/

	cUnitEntry unitEntry;
	vector<cUnitEntry> unitList;
	vec2 anchor = getChunkCenter(pos);

	//access.lock();
	//game.access.lock();

	for (int i = 0; i < game.unitCounter; i++)
	{
		if (game.unit[i].chunkPos == pos && !game.unit[i].hasRef(REF_UNIT_NOUNLOAD))
		{
			unitEntry.pos = game.unit[i].pos - anchor;
			unitEntry.type = game.unit[i].type;
			unitEntry.globalId = game.unit[i].globalId;
			unitList.push_back(unitEntry);
		}
	}
	save.flushListToFile(map[pos.x][pos.y].type, unitList, save.getChunkFilePath(pos));

	//game.access.unlock();
	//access.unlock();
}

void cWorld::loadChunk(vec2i pos)
{
	/*
	WARNING:
	This function is only to be used on the server side!
	Client must request the chunk update from the server.
	*/
	//access.lock();
	//game.access.lock();

	int attachIndex = -1;
	vec2 anchor = getChunkCenter(pos);

	// Adding units
	int id = 0;
	vector<cUnitEntry> unitList = getChunkUnitList(pos);
	for (int i = 0; i < (int)unitList.size(); i++)
	{
		if (game.getUnitId(unitList[i].globalId) == -1)// && )
		{
			id = game.addUnit(unitList[i].type, anchor + unitList[i].pos, -1, -1, false);
			// Assign global id if the object is loaded for the first time
			if (unitList[i].globalId != -1) {
				game.unit[game.getUnitId(id)].globalId = unitList[i].globalId;
			}
			// Tell unit to which chunk it belongs
			game.unit[game.getUnitId(id)].chunkPos = pos;
			if (pos != world.getChunkInPos(anchor + unitList[i].pos))
			{
				cout << "ALARMA: " << pos.x << "; " << pos.y << " / " << world.getChunkInPos(game.unit[game.getUnitId(id)].pos).x << "; "
					<< world.getChunkInPos(game.unit[game.getUnitId(id)].pos).y << endl;
			}
		}
	}
	map[pos.x][pos.y].isLoaded = true;

	//game.access.unlock();
	//access.unlock();
}

void cWorld::unloadChunk(vec2i pos)
{
	if (!map[pos.x][pos.y].isLoaded) { cout << "[ERROR] Chunk (" << pos.x << "; " << pos.y << ") is not loaded!"; return; }
	//access.lock();
	//game.access.lock();
	for (int i = 0; i < game.unitCounter; i++)
	{
		if (game.unit[i].chunkPos == pos && !game.unit[i].hasRef(REF_UNIT_NOUNLOAD))
		{
			game.removeUnit(game.unit[i].globalId, false);
			i -= 1;
		}
	}
	map[pos.x][pos.y].isLoaded = false;
	//game.access.unlock();
	//access.unlock();
}

vector<cUnitEntry> cWorld::getChunkUnitList(vec2i pos)
{
	return save.getListFromFile(save.getChunkFilePath(pos));
}

vector<cUnitEntry> cWorld::getBlueprintUnitList(int index)
{
	vector<cUnitEntry> retVal;
	if (index == -1) { cout << "[ERROR] Invalid blueprint id!" << "\n"; return retVal; }
	return save.getListFromFile("Data//Blueprints//" + blueprint[index].name);
}