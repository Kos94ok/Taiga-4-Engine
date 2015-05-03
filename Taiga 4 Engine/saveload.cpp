
#include "main.h"

void cSave::flushListToFile(vector<cUnitEntry> list, string filename)
{
	ofstream file;
	file.open(filename);
	if (!file.good()) { cout << "[ERROR] Can't open file " + filename << endl; return; }
	// Writing data
	file << 1 << "\n";
	file << "[Units]" << "\n";
	for (int i = 0; i < (int)list.size(); i++)
	{
		file << list[i].globalId << "\n";
		file << list[i].type << "\n";
		file << list[i].pos.x << "\n";
		file << list[i].pos.y << "\n";
	}
	file.close();
}

vector<cUnitEntry> cSave::getListFromFile(string filename)
{
	vector<cUnitEntry> retVal;
	char buffer[256];
	string buf;
	cUnitEntry entry;
	ifstream file;
	file.open(filename);
	if (file.good())
	{
		// Skipping to unit lines
		do
		{
			file.getline(buffer, 256);
			buf = buffer;
		} while (!file.eof() && buf != "[Units]");
		// Reading units
		while (!file.eof())
		{
			// Global Id
			file.getline(buffer, 256);	buf = buffer;
			if (buf.length() > 0)
			{
				entry.globalId = math.stringToInt(buffer);
				// Type
				file.getline(buffer, 256);	buf = buffer;
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