
#include "prerender.h"
#include "main.h"
#include "console.h"
#include "settings.h"
#include "camera.h"
#include "game.h"

void cPreRender::updateUnits()
{
	game.access.lock();
	mutex.renderUnits.lock();
	preRender.units.ready = true;
	preRender.units.queue.clear();

	bool cameraIntersection;
	float step = 20.00f;
	sf::RenderStates renderState;
	// Check step
	if (settings.unitDepthCheck == 0) { step = 10.00f; }
	else if (settings.unitDepthCheck == 1) { step = 5.00f; }
	else if (settings.unitDepthCheck == 2) { step = 3.00f; }
	else if (settings.unitDepthCheck == 3) { step = 1.00f; }

	float cameraLeft = camera.pos.x;
	float cameraRight = camera.res.x + camera.pos.x;
	float cameraTop = camera.pos.y;
	float cameraBot = camera.res.y + camera.pos.y;
	// Render distance
	if (core.advancedDebug)
	{
		cameraLeft -= 5000;	cameraRight += 5000;
		cameraTop -= 5000;	cameraBot += 5000;
	}
	else if (settings.unitRenderDistance == 0)
	{
		cameraLeft -= 100;	cameraRight += 100;
		cameraTop -= 50;	cameraBot += 350;
	}
	else if (settings.unitRenderDistance == 1)
	{
		cameraLeft -= 200;	cameraRight += 200;
		cameraTop -= 100;	cameraBot += 450;
	}
	else if (settings.unitRenderDistance == 2)
	{
		cameraLeft -= 300;	cameraRight += 300;
		cameraTop -= 100;	cameraBot += 550;
	}
	else if (settings.unitRenderDistance == 3)
	{
		cameraLeft -= 400;	cameraRight += 400;
		cameraTop -= 150;	cameraBot += 750;
	}

	// Random initialization
	float unitLeft, unitRight;
	sf::FloatRect camRect(camera.pos.x, camera.pos.y, camera.res.x, camera.res.y);
	sf::FloatRect objRect;

	for (float y = cameraTop; y < cameraBot; y += step)
	{
		for (int i = 0; i < game.unitCounter; i++)
		{
			unitLeft = game.unit[i].pos.x - game.unit[i].center.x;
			unitRight = unitLeft + game.unit[i].size.x;

			if (!game.unit[i].hasRef(REF_UNIT_NORENDER)
				&& ((game.unit[i].pos.y >= y && game.unit[i].pos.y < y + step && unitRight >= cameraLeft && unitLeft <= cameraRight && !game.unit[i].hasRef(REF_UNIT_ALWAYSVISIBLE))
				|| (game.unit[i].hasRef(REF_UNIT_ALWAYSVISIBLE) && y == cameraTop)))
			{
				// Check if unit is already in the queue
				bool found = false;
				for (int checkVal : preRender.units.queue)
				{
					if (checkVal == i)
					{
						found = true;
						break;
					}
				}
				// Don't allow repeating
				if (!found) { preRender.units.queue.push_back(i); }
			}
		}
	}
	mutex.renderUnits.unlock();
	game.access.unlock();
}

void preRenderMain()
{
	if (core.serverMode) { return; }

	int threadId = 10;
	console << "[RENDER] Pre-render thread started" << "\n";
	while (!core.thread_shutdown[threadId])
	{
		if (!preRender.units.ready) { preRender.updateUnits(); }
		core.thread_antifreeze[threadId] = 0;
		Sleep(5);
	}
	console << "[RENDER] Cleaning up" << "\n";
}