
#include "define.h"

class cComponent
{
public:
	std::string type;
	int count;

	cComponent() {
		type = "";
		count = 1;
	}

	cComponent(std::string item) {
		type = item;
		count = 1;
	}

	cComponent(const char* item) {
		type = item;
		count = 1;
	}

	cComponent(std::string item, int amount) {
		type = item;
		count = amount;
	}

	cComponent& operator=(std::string item) {
		type = item;
		count = 1;
		return *this;
	}
};

class cRecipe
{
public:
	cComponent ingr[LIMIT_COMPONENTS];
	cComponent result;
	int ingrCount;

	int resourceBalance;

	cRecipe() {
		ingrCount = 1;
		resourceBalance = 0;
	}
};

class cCraft
{
public:
	cItemContainer cont;
	cItemContainer resultCont;
	int recipeCounter;
	cRecipe recipe[LIMIT_RECIPES];

	cCraft() {
		recipeCounter = 0;
		cont.itemLimit = LIMIT_COMPONENTS;
		resultCont.itemLimit = 1;
	}

	void addRecipe(cComponent result, int resBalance,  cComponent ingrA,
				cComponent ingrB = "", cComponent ingrC = "", cComponent ingrD = "",	 cComponent ingrE = "",
				cComponent ingrF = "", cComponent ingrG = "", cComponent ingrH = "", cComponent ingrI = "");
	void checkActiveRecipe();
	void updateCraftAmounts();
	void loadRecipes();
	void getResult(bool craftAll);
	int getActiveRecipeId();
	int getActiveRecipeRepeats();
};