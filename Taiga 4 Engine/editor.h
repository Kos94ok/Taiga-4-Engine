
#pragma once
#include "precomp.h"

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

	int autogenType;

	void enable();
	void initialize();

	void saveBlueprint(std::string name);
	void loadBlueprint(std::string name);

	void genBlueprint();
	void genBlueprint_Normal();
	void genBlueprint_Blocked();

	cEditor() {
		autogenType = CHUNK_NORMAL;
	}
};

extern cEditor editor;