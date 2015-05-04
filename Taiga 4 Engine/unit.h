
#include "define.h"

class cOrder
{
public:
	int type;
	int targetObject;
	sf::Vector2f targetPos;
};

class cAnimDirection
{
public:
	int tex;
	int frameCount;
	float frameDelay;

	cAnimDirection() {
		tex = -1;
		frameCount = 1;
		frameDelay = 0.25f;
	}
};

class cAnimation
{
public:
	cAnimDirection up;
	cAnimDirection upDiag;
	cAnimDirection side;
	cAnimDirection downDiag;
	cAnimDirection down;
};

class cCurrentAnim
{
public:
	int type;
	int curFrame;
	float curFrameTimer;

	cCurrentAnim();
	void play(int animType, bool forceInterrupt = false);
	void reset();
};

class cLightSource
{
public:
	float power;
	int texture;

	float flickerMod;
	float flickerTime;
	float flickerCurTime;

	cLightSource() {
		power = 0.00f;
		texture = -1;
		flickerMod = 0.00f;
		flickerCurTime = 0.00f;
	}
};

class cShader
{
public:
	int id;
	int bloomSampleCount;
	float bloomSampleOffset;

	cShader() {
		id = -1;
		bloomSampleCount = 1;
		bloomSampleOffset = 0.005f;
	}
};

class cCharacter : public cReference
{
public:
	int owner;
	int globalId;
	std::string type;
	sf::Vector2f pos;
	sf::Vector2f center;
	sf::Vector2f size;
	sf::Vector2f shadowOffset;
	float interactDistance;
	float collisionDistance;
	float flyingHeight;

	cCurrentAnim anim;
	cAnimation animData[LIMIT_ANIMATIONS];

	cLightSource light;
	cShader shader;
};

class cUnit : public cCharacter
{
public:
	int orderCounter;
	cOrder order[LIMIT_ORDERS];
	float actionTimer;
	sf::Vector2i chunkPos;

	int addOrder_moveto(sf::Vector2f target, bool overwrite = true);
	int addOrder_moveto_path(sf::Vector2f target, bool overwrite = true);
	int addOrder_pickup(int target, bool overwrite = true);
	int addOrder_harvest(int target, bool overwrite = true);
	void removeOrder(int id);
	void removeAllOrders();

	float health;
	float displayHealth;
	float maxHealth;
	float facingAngle;
	float movementSpeed;
	cAnimDirection getCurrentAnimDirection();
	cItemContainer container;

	float resource;
	void addResource(float d);
	void setResource(float value);

	void moveTo(sf::Vector2f newPos);
	void addHealth(float d);
	void addMaxHealth(float d);
	void setHealth(float hp);
	void setMaxHealth(float hp);
	void addItem(std::string type, int count = 1);
	void removeItem(std::string type, int count = -1);
	void removeItem(int id, int count = -1);
	void updateFacing();
	void updateAction();
	void updateAnimation();

	cUnit() {
		flyingHeight = 0.00f;
		actionTimer = 0.00f;
		resource = 0.00f;
		interactDistance = 0.00f;
		collisionDistance = 0.00f;
		orderCounter = 0;
		facingAngle = 0.00f;
		health = 50.00f;
		displayHealth = health;
		maxHealth = 100.00f;
		owner = -1;
	}
};