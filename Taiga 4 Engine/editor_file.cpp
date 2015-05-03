
#include "main.h"

void cEditor::saveBlueprint(string name)
{
	cUnitEntry unitEntry;
	vector<cUnitEntry> unitList;
	// Looking for the path
	string path = "Data//Blueprints//" + name;
	if (path.substr(path.length() - 10) != ".blueprint") { path += ".blueprint"; }
	// Creating unit list
	for (int i = 0; i < game.unitCounter; i++)
	{
		if (!game.unit[i].hasRef(REF_UNIT_NOSAVE))
		{
			unitEntry.pos = game.unit[i].pos;
			unitEntry.type = game.unit[i].type;
			unitEntry.globalId = game.unit[i].globalId;
			unitList.push_back(unitEntry);
		}
	}
	save.flushListToFile(unitList, path);
}

void cEditor::loadBlueprint(string name)
{
	char buffer[256];
	string buf, type;
	vec2 position;
	// Looking for the path
	string path = "Data//Blueprints//" + name;
	if (path.substr(path.length() - 10) != ".blueprint") { path += ".blueprint"; }
	// Opening file
	ifstream file;
	file.open(path);
	if (file.good())
	{
		// Reading
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
				type = buffer;
				// Pos X
				file.getline(buffer, 256);	buf = buffer;
				position.x = math.stringToInt(buffer);
				// Pos Y
				file.getline(buffer, 256);	buf = buffer;
				position.y = math.stringToInt(buffer);
				// Adding units
				game.addUnit(type, position);
			}
		}
		// Closing
		file.close();
	}
}