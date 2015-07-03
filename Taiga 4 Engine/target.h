
#pragma once
#include "precomp.h"

class cTarget
{
public:
	bool active;
	bool activeBuild;
	int mode;
	int buildRef;
	int forButton;

	void enable_forButton(int elementId);
	void enable_building(int ref);
	void apply();
	void reset();
	
	cTarget() {
		active = false;
		activeBuild = false;
	}
};

extern cTarget target;