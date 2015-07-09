
#include "visual.h"
#include "math.h"
#include "database.h"

void cVisual::setMousePointer(int tex)
{

}

void cVisual::enableProgressBar(float time)
{
	progress.enabled = true;
	progress.manualMode = false;
	progress.timeCurrent = 0.00f;
	progress.timeMaximum = time;
	progress.fadeTimer = 0.00f;
}

void cVisual::enableProgressBarManual(float time, int stepCount)
{
	enableProgressBar(time);
	progress.manualMode = true;
	progress.manualModeStep = time / stepCount;
}

void cVisual::advanceProgressBar()
{
	progress.timeCurrent += progress.manualModeStep;
	if (progress.timeCurrent > progress.timeMaximum) {
		progress.timeCurrent = progress.timeMaximum;
	}
}

int cProgressBar::getTexFrameCountX() { return 9; }
int cProgressBar::getTexFrameCountY() { return 8; }

float cProgressBar::getCurrentAlpha() {
	if (enabled)
	{
		return fadeTimer / value.progressBarFadeInTimer;
	}
	else
	{
		return fadeTimer / value.progressBarFadeOutTimer;
	}
}

vec2i cProgressBar::getCurrentFrame()
{
	int frameLimitX = getTexFrameCountX();
	int frameLimitY = getTexFrameCountY();

	float pos = timeCurrent / timeMaximum;
	int curFrame = math.round(pos * (frameLimitX * frameLimitY));
	int curFrameY = curFrame / frameLimitX;
	int curFrameX = curFrame - curFrameY * frameLimitX;
	if (curFrame >= frameLimitX * frameLimitY) {
		curFrameX = frameLimitX - 1;
		curFrameY = frameLimitY - 1;
	}

	return vec2i(curFrameX, curFrameY);
}

vec2i cProgressBar::getTexFrameSize()
{
	sf::Vector2u fullSize = visual.gameTex[database.texture[TEX_MOUSE_PROGRESS]].handle.getSize();
	return vec2i(fullSize.x / getTexFrameCountX(), fullSize.y / getTexFrameCountY());
}

void cVisual::disableProgressBar()
{
	progress.enabled = false;
	progress.fadeTimer = value.progressBarFadeOutTimer;
}