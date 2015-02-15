
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
			// Amount of attach points
		/*file << 4 << endl;
		file << -300 << endl;
		file << 0 << endl;
		file << DIRECTION_LEFT << endl;
		file << 300 << endl;
		file << 0 << endl;
		file << DIRECTION_RIGHT << endl;
		file << 0 << endl;
		file << 300 << endl;
		file << DIRECTION_UP << endl;
		file << 0 << endl;
		file << -300 << endl;
		file << DIRECTION_DOWN << endl;*/
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
}