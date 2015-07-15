
#pragma once
#include "precomp.h"
#include "define.h"
#include "audio.h"
#include "refSystem.h"
#include "ai.h"
#include "item.h"
#include "animation.h"
#include "buff.h"

class cUnitSoundbank
{
public:
	cSound idle;
};

class cOrder
{
public:
	int type;
	int targetObject;
	int paramA;
	sf::Vector2f targetPos;
};

class cLightSource
{
public:
	float power;
	int texture;

	int priority;
	bool directional;
	float flickerMod;
	float flickerTime;
	float flickerCurTime;

	cLightSource() {
		priority = 0;
		power = 0.00f;
		texture = -1;
		flickerMod = 0.00f;
		flickerCurTime = 0.00f;
		directional = false;
	}

	cLightSource* operator=(int textureIndex)
	{
		texture = textureIndex;
		return this;
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

class cLifeTimer
{
public:
	bool enabled;
	float time;

	cLifeTimer()
	{
		enabled = false;
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
	sf::Vector2f selectionOffset;
	float interactDistance;
	float collisionDistance;
	float flyingHeight;
	int selectionPriority;

	bool animAvailable(int animType);

	cCurrentAnim anim;
	cAnimDisplay animDisplay;
	cAnimation animData[LIMIT_ANIMATIONS];
	cLightSource light;
	cShader shader;
};

class cUnit : public cCharacter
{
public:
	bool toRemove;
	int orderCounter;
	cOrder order[LIMIT_ORDERS];
	float actionTimer;
	sf::Vector2i chunkPos;

	int addOrder_moveto(sf::Vector2f target, bool overwrite = true);
	int addOrder_moveto_path(sf::Vector2f target, bool overwrite = true);
	int addOrder_pickup(int target, bool overwrite = true);
	int addOrder_harvest(int target, bool overwrite = true, int powerLevel = POWER_HAND);
	int addOrder_packunit(int target, bool overwrite = true);
	int addOrder_death(bool overwrite = true);
	void removeOrder(int id);
	void clearOrders();
	void removeAllOrders();

	float health;
	float displayHealth;
	float maxHealth;
	float facingAngle;
	float targetFacingAngle;
	float rotateSpeed;
	float movementSpeed;
	cAIBrain ai;
	cBuffList buff;
	cLifeTimer lifeTimer;
	cItemContainer container;
	cDropContainer drop;
	cUnitSoundbank sound;

	float resource;
	void addResource(float d);
	void setResource(float value);

	void moveTo(sf::Vector2f newPos);
	void rotateTo(float newAngle);
	void addHealth(float d);
	void addMaxHealth(float d);
	void setHealth(float hp);
	void setMaxHealth(float hp);
	void addItem(std::string type, int count = 1, bool toLog = true);
	void removeItem(std::string type, int count = -1, bool toLog = true);
	void removeItem(int id, int count = -1, bool toLog = true);
	void addBuff(int type, float duration = -1.00f, int power = 1);
	void removeBuff(int type);
	bool hasBuff(int type);
	void setLifeTimer(float time);
	void resetLifeTimer();
	void updateFacing();
	void updateAction();
	void updateAnimation();
	void updateDisplayAnim();

	cUnit() {
		toRemove = false;
		flyingHeight = 0.00f;
		actionTimer = 0.00f;
		resource = 0.00f;
		interactDistance = 0.00f;
		collisionDistance = 0.00f;
		orderCounter = 0;
		facingAngle = 0.00f;
		targetFacingAngle = facingAngle;
		health = 50.00f;
		displayHealth = health;
		maxHealth = 100.00f;
		owner = -1;
		selectionPriority = 0;
		rotateSpeed = 360.00f;
	}
};