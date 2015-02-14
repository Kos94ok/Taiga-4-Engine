
#include "main.h"

vec2 cWorld::applyBlueprint(vec2 position, int index, cBlueprintAttach *attach)
{
	int attachIndex = -1;
	vec2 anchor = position;
	// Getting the attach index
	for (int i = 0; i < (int)blueprint[index].attach.size(); i++)
	{
		if ((blueprint[index].attach[i].direction == DIRECTION_LEFT && attach->direction == DIRECTION_RIGHT)
			|| (blueprint[index].attach[i].direction == DIRECTION_RIGHT && attach->direction == DIRECTION_LEFT)
			|| (blueprint[index].attach[i].direction == DIRECTION_UP && attach->direction == DIRECTION_DOWN)
			|| (blueprint[index].attach[i].direction == DIRECTION_DOWN && attach->direction == DIRECTION_UP))
		{
			attachIndex = i;
			i = (int)blueprint[index].attach.size();
		}
	}
	// Ignore if there is no place for attachments
	if (attachIndex != -1)
	{
		// Calculating the anchor
		anchor = position + vec2(blueprint[index].attach[attachIndex].x - attach->x, blueprint[index].attach[attachIndex].y - attach->y);

		// Adding units
		vector<cUnitEntry> unitList = getBlueprintUnitList(index);
		for (int i = 0; i < (int)unitList.size(); i++)
		{
			game.addUnit(unitList[i].type, anchor + unitList[i].pos);
		}
	}
	// Returning position
	return anchor;
}

vector<cUnitEntry> cWorld::getBlueprintUnitList(int index)
{
	char buffer[256];
	string buf;
	cUnitEntry entry;
	vector<cUnitEntry> retVal;
	ifstream file;
	file.open("Data//Blueprints//" + blueprint[index].name);
	if (file.good())
	{
		// Skipping attach lines
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