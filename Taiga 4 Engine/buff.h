
#include "precomp.h"

class cBuff
{
public:
	int type;
	int power;
	float duration;

	cBuff() {
		type = 0;
		power = 0;
		duration = 0.00f;
	}
};

class cBuffList
{
public:
	std::vector<cBuff> list;

	void add(int type, float duration = -1.00f, int power = 1);
	void remove(int type);
	void removeById(int id);
	int getPower(int type);
	float getDuration(int type);
};