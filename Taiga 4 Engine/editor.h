
#include "define.h"

class cEditorSelection
{
public:
	int count;
	int unit[LIMIT_SELECTION];

	void add(int target);
	void remove(int target);
	void clear();
	bool isSelected(int target);

	cEditorSelection() {
		count = 0;
	}
};

class cEditor
{
public:
	cEditorSelection sel;

	void enable();
	void initialize();

	void saveBlueprint(std::string name);
	void loadBlueprint(std::string name);

	cEditor() {
		
	}
};