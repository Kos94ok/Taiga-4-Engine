
#include "main.h"

void cEditor::saveBlueprint(string name)
{
	// Looking for the path
	string path = "Data//Blueprints//" + name;
	if (path.substr(path.length() - 10) != ".blueprint") { path += ".blueprint"; }
	// Opening file
	ofstream file;
	file.open(path);
	if (file.good())
	{
		// Writing
			// Other objects
		file << "[Units]" << endl;
		for (int i = 0; i < game.unitCounter; i++)
		{
			if (!game.unit[i].hasRef(REF_UNIT_NOSAVE))
			{
				file << game.unit[i].type << endl;
				file << game.unit[i].pos.x << endl;
				file << game.unit[i].pos.y << endl;
			}
		}

		// Closing the file
		file.close();
	}
	else { cout << "[ERROR] Can't create the file " << path << endl; }
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