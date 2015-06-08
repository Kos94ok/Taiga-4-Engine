
#include "define.h"

class cItemDismantle
{
public:
	int itemCounter;
	std::string item[LIMIT_DISMANTLE];
	int amount[LIMIT_DISMANTLE];

	void add(std::string type, int count) {
		item[itemCounter] = type;
		amount[itemCounter++] = count;
	}

	cItemDismantle() {
		itemCounter = 0;
	}
};

class cItemConsume
{
public:
	int healthBalance;

	cItemConsume(){
		healthBalance = 0;
	}
};

class cItem : public cReference
{
public:
	int globalId;
	int category;
	std::string type;
	std::string displayName;
	int descrLinesUsed;
	std::string description[LIMIT_DESCR];

	bool equipped;
	float weight;
	cIcon icon;
	cItemConsume consume;
	cItemDismantle dismantle;

	void addDescrLine(std::string line);
	void setName(std::string txt);

	void use();

	cItem() {
		equipped = false;
		weight = 0.10f;
		descrLinesUsed = 0;
		category = CATEGORY_OTHER;
	}
};

class cItemContainer
{
public:
	float resource;

	bool openable;
	int itemLimit;
	int itemCounter;
	cItem item[LIMIT_ITEMS];
	int amount[LIMIT_ITEMS];

	int add(cItem target, int count = 1);
	int add(std::string type, int count = 1);
	// Get item from global id
	cItem& get(int id);
	// Get item from type
	cItem& get(std::string type, int begin = 0);
	// Get amount of items with that type
	int getAmount(std::string type);
	// Remove item using it's global id
	bool remove(int id, int count = -1);
	// Remove item with given type
	bool remove(std::string type, int count = -1, int begin = 0);

	// Get list number from global id
	int getId(int id);
	// Get list number from item type
	int getId(std::string type, int begin = 0);

	void sort(int sortType);

	cItemContainer() {
		openable = false;
		resource = 0.00f;
		itemLimit = 100;
		itemCounter = 0;
	}
};