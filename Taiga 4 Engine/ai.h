
#pragma once
void AICoreMain();

class cAIBrain
{
public:
	// Core
	bool enabled;
	int thinkTimer;
	std::function<void(int globalId)> thinkFunc;

	// Intelligence
	int intTarget;

	// Constructors
	cAIBrain() {
		intTarget = -1;
		thinkTimer = 0;
		enabled = false;
	}
	cAIBrain(std::function<void(int globalId)> function) {
		enabled = true;
		thinkFunc = function;
	}
	cAIBrain& operator= (std::function<void(int globalId)> function) {
		enabled = true;
		thinkFunc = function;

		return *this;
	}
};

class cAI
{
public:
	bool enabled;

	int localTime;
	
	// Perform regular actions
	void tick(int elapsedTime);
	// Enable AI
	void enable() { enabled = true; }
	// Disable AI
	void disable() { enabled = false; }

	// Thinking functions
	static void think_enemy(int globalId);

	cAI()
	{
		enabled = false;
		localTime = 0;
	}
};

extern cAI ai;