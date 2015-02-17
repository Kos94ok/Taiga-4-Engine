
#include "main.h"

void cWorld::applyBlueprint(vec2i position, int type)
{
	vector<int> correctBlueprints;
	ifstream in;
	ofstream out;

	// Getting all correct blueprints
	for (int i = 0; i < (int)blueprint.size(); i++)
	{
		if (blueprint[i].type == type) { correctBlueprints.push_back(i); }
	}
	if (correctBlueprints.size() > 0)
	{
		// Getting random correct blueprint
		int bpIndex = correctBlueprints[math.rand(0, (int)correctBlueprints.size() - 1)];
		map[position.x][position.y].blueprint = bpIndex;

		in.open("Data//Blueprints//" + blueprint[bpIndex].name);
		out.open("Savefiles//" + save.worldName + "//" + to_string(position.x) + "-" + to_string(position.y) + ".chunk");
		out << in.rdbuf();
	}
	else { cout << "[ERROR] Can't apply blueprint type " << type << " to (" << position.x << "; " << position.y << ")!" << "\n"; }
}

bool cWorld::isChunkLoaded(vec2i pos)
{
	return map[pos.x][pos.y].isLoaded;
}

bool cWorld::isChunkViable(vec2i pos)
{
	float viableDist = 1000.00f;
	vec2 chunkCenter = getChunkCenter(pos);
	// Local player
	if (client.connected && math.getDistance(camera.pos + vec2(camera.res.x / 2, camera.res.y / 2), chunkCenter) < viableDist) {
		return true;
	}
	// Remote players
	if (core.localServer || core.serverMode)
	{
		for (int i = 0; i < LIMIT_SERVER_PLAYERS; i++)
		{
			if (server.player[i].connected && math.getDistance(server.player[i].camPos
				+ vec2(server.player[i].camRes.x / 2, server.player[i].camRes.y / 2), chunkCenter) < viableDist)
			{
				return true;
			}
		}
	}
	return false;
}

vec2i cWorld::getChunkInPos(vec2 pos)
{
	return vec2i(floor(pos.x / LIMIT_CHUNKSIZE), floor(pos.y / LIMIT_CHUNKSIZE));
}

vec2 cWorld::getChunkCenter(vec2i pos)
{
	return vec2((float)pos.x * LIMIT_CHUNKSIZE + LIMIT_CHUNKSIZE / 2, (float)pos.y * LIMIT_CHUNKSIZE + LIMIT_CHUNKSIZE / 2);
}