
#pragma once
#include "precomp.h"
#include "define.h"
#include "unit.h"
#include "item.h"
#include "particle.h"
#include "UI.h"
#include "util.h"

//extern cVisual visual;

class cDatabaseKey
{
public:
	std::string name;
	std::string value;
};

class cDatabaseExternal
{
public:
	std::vector<std::string> fileList;
	bool getFileList();

	bool parseFile(int index);
	bool getNextKey(std::ifstream *file, cDatabaseKey *key);
};

class cDatabase
{
public:
	cMutex access;

	cDatabaseExternal ext;
	cUnit unit[LIMIT_DB_UNIT];
	cItem item[LIMIT_DB_ITEM];
	cUIElement uiElement[LIMIT_DB_UIELEMENTS];
	cParticleUnit particle[LIMIT_DB_PARTICLES];
	int texture[LIMIT_DB_TEXTURES];

	void init();
	void loadUnits();
	void loadItems();
	void loadUI();
	void loadParticles();
	void loadTextures();
	void loadExternal();
	void generateTextures();
	void clear();

	cUnit& getUnit(std::string type);
	cItem& getItem(std::string type);
	cItem& findItem(int ref);
	cUIElement& getUIElement(std::string type);
	cParticleUnit& getParticle(std::string type);

	bool isItemGood(std::string type);
};

extern cDatabase database;