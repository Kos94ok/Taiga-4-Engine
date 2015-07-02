
#pragma once
#include "precomp.h"

class cItemEntry
{
public:
	std::string type;
	int count;
};

class cUnitEntry
{
public:
	int globalId;
	std::string type;
	sf::Vector2f pos;
	std::vector<cItemEntry> itemList;

	cUnitEntry() {
		globalId = -1;
	}
};

class cBlueprintHeader
{
	/*
	cBlueprintHeader is used to fetch the blueprint header data from the file.
	*/
public:
	int type;
	bool isValid;

	cBlueprintHeader() {
		type = -1;
		isValid = true;
	}

	cBlueprintHeader(int i) {
		type = i;
		isValid = true;
	}
};

class cSave
{
public:
	std::string savefileName;
	std::string worldName;

	std::string getChunkFilePath(sf::Vector2i pos);
	cBlueprintHeader getHeaderFromFile(std::string filename);
	std::vector<cUnitEntry> getListFromFile(std::string filename);
	void flushListToFile(cBlueprintHeader header, std::vector<cUnitEntry> list, std::string filename);

	cSave() {
		worldName = "world";
		savefileName = "temp";
	}
};

extern cSave save;