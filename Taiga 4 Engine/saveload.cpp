
#include "main.h"

void cSave::flushListToFile(cBlueprintHeader header, vector<cUnitEntry> list, string filename)
{
	ofstream file;
	file.open(filename);
	if (!file.good()) { cout << "[cSave::flushListToFile] Can't open file " + filename << endl; return; }
	// Writing data
	file << "Type = " << header.type << "\n";
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

cBlueprintHeader cSave::getHeaderFromFile(string filename)
{
	cBlueprintHeader retVal;
	char buffer[256];
	string buf;
	cUnitEntry entry;
	ifstream file;
	file.open(filename);
	if (file.good())
	{
		file.getline(buffer, 256);	buf = buffer;
		// Reading until some category appears
		while (buf.substr(0, 1) != "[" && !file.eof())
		{
			// If the line is ok, parsing the line
			if (buf.length() > 0)
			{
				if (buf.substr(0, 4) == "Type")
				{
					retVal.type = math.stringToInt(buf.substr(7));
				}
			}
			else { cout << "[cSave::getHeaderFromFile] Unexpected end of file (" << filename << ")\n"; retVal.isValid = false; }
			// Reading the next line
			file.getline(buffer, 256);	buf = buffer;
		}
		// Closing
		file.close();
	}
	return retVal;
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