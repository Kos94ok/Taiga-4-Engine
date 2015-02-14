
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
	int countCorrect = 0, countWrong = 0, value = -1;
	char buf[256];
	cBlueprint print;
	cBlueprintAttach attach;
	ifstream file;
	vector<string> list = getBlueprintList();
	for (int i = 0; i < (int)list.size(); i++)
	{
		print.name = list[i];
		print.attach.clear();

		// Opening the file
		file.open("Data//Blueprints//" + list[i]);
		if (file.good())
		{
			// Reading the attach point count
			file.getline(buf, 256);
			value = math.stringToInt(buf);

			// For every attach point
			for (int j = 0; j < value; j++)
			{
				// Pos X
				file.getline(buf, 256);	value = math.stringToInt(buf);
				attach.x = value;
				// Pos Y
				file.getline(buf, 256);	value = math.stringToInt(buf);
				attach.y = value;
				// Direction
				file.getline(buf, 256);	value = math.stringToInt(buf);
				attach.direction = value;
				// Pushing to array
				print.attach.push_back(attach);
			}
			file.close();
			// Pushing to array
			blueprint.push_back(print);
			countCorrect += 1;
		}
		else { countWrong += 1; }
	}
	cout << "[MAIN] Blueprints analyzed: " << (int)list.size() << ". Correct: " << countCorrect << ", Wrong: " << countWrong << endl;
}