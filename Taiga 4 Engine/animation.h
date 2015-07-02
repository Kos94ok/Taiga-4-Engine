
#pragma once
class cAnimDirection
{
public:
	int tex;
	int texShadow;
	int frameCount;
	float frameDelay;

	cAnimDirection() {
		tex = -1;
		texShadow = -1;
		frameCount = 1;
		frameDelay = 0.25f;
	}
};

class cAnimDisplay
{
public:
	cAnimDirection data;
	int texMod;
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