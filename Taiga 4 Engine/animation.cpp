
#include "main.h"

cAnimDirection cUnit::getCurrentAnimDirection()
{
	int animId = anim.type;
	int curFrame = anim.curFrame;

	bool upDownAvailable = (animData[animId].up.tex != -1 && animData[animId].down.tex != -1);
	// Right
	if ((upDownAvailable && (facingAngle >= 315.00f || facingAngle < 45.00f))
		|| (!upDownAvailable && (facingAngle >= 270.00 || facingAngle < 90.00f))) {
		return animData[animId].side;
	}
	// Up
	else if (upDownAvailable && (facingAngle >= 45.00f && facingAngle < 135.00f)) {
		return animData[animId].up;
	}
	// Left
	else if ((upDownAvailable && (facingAngle >= 135.00f && facingAngle < 225.00f))
		|| (!upDownAvailable && (facingAngle >= 90.00f || facingAngle < 270.00f))) {
		return animData[animId].side;
	}
	// Down
	else if (upDownAvailable && (facingAngle >= 225.00f && facingAngle < 315.00f)) {
		return animData[animId].down;
	}
	return animData[animId].side;
}

cCurrentAnim::cCurrentAnim()
{
	reset();
}

void cCurrentAnim::reset()
{
	type = ANIM_IDLE;
	curFrame = 0;
	curFrameTimer = 0.00f;
}

void cCurrentAnim::play(int animType, bool forceInterrupt)
{
	// Don't interrupt the animation if not needed
	if (animType != type || forceInterrupt)
	{
		reset();
		type = animType;
	}
}