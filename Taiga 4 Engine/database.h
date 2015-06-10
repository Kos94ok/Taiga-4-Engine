
#include "define.h"

extern cVisual visual;

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
	int texture[LIMIT_DB_TEXTURES];

	void init();
	void loadUnits();
	void loadItems();
	void loadUI();
	void loadTextures();
	void loadExternal();

	cUnit& getUnit(std::string type);
	cItem& getItem(std::string type);
	cUIElement& getUIElement(std::string type);

	bool isItemGood(std::string type);
};