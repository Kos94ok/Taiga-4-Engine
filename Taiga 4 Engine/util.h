
class cColor
{
public:
	int r;
	int g;
	int b;
	int a;

	cColor() {
		r = 0;
		g = 0;
		b = 0;
		a = 255;
	};

	cColor(int R, int G, int B, int A) {
		r = R;
		g = G;
		b = B;
		a = A;
	}
};

class cUtil
{
public:
	void detectVideoCard();
	int getInventorySortingType();
	void checkLogFiles();
};

class cValue
{
public:
	const float rifleDamage = 10.00f;
	const float rifleMaxDistance = 1750.00f;

	const float enemyDamage = 10.00f;
};

class cMutex
{
public:
	sf::Mutex mutex;

	void lock() {
		mutex.lock();
	}
	void unlock() {
		mutex.unlock();
	}
};