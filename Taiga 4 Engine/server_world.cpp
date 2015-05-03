
#include "main.h"

void serverWorldOrders(int elapsedTime)
{
	game.access.lock();
	float timevar = (float)elapsedTime / 1000;
	timevar *= core.timeModifier;
	float angle, offsetX, offsetY;
	for (int i = 0; i < game.unitCounter; i++)
	{
		if (game.unit[i].orderCounter > 0)
		{
			// Action timer
			if (game.unit[i].actionTimer > 0.00f) {
				game.unit[i].actionTimer -= timevar;
				if (game.unit[i].actionTimer < 0.00f) { game.unit[i].actionTimer = 0.00f; }
			}
			else
			{
				// Move to point order
				if (game.unit[i].order[0].type == ORDER_MOVETO)
				{
					// Calculating the point
					if (math.getDistance(game.unit[i].pos.x, game.unit[i].pos.y, game.unit[i].order[0].targetPos.x, game.unit[i].order[0].targetPos.y)
						> game.unit[i].movementSpeed * timevar)
					{
						angle = math.getAngle(game.unit[i].pos.x, game.unit[i].pos.y, game.unit[i].order[0].targetPos.x, game.unit[i].order[0].targetPos.y);
						offsetX = game.unit[i].movementSpeed * timevar * cos(angle * math.DEGTORAD);
						offsetY = game.unit[i].movementSpeed * timevar * sin(angle * math.DEGTORAD);
					}
					else
					{
						offsetX = game.unit[i].order[0].targetPos.x - game.unit[i].pos.x;
						offsetY = game.unit[i].order[0].targetPos.y - game.unit[i].pos.y;
						game.unit[i].removeOrder(0);
					}
					// Moving the unit to correct position
					game.unit[i].pos.x += offsetX;
					game.unit[i].pos.y += offsetY;
					if (game.unit[i].globalId == client.unit)
					{
						if (camera.lockedToCharacter) { camera.moveto(sf::Vector2f(game.unit[i].pos.x, game.unit[i].pos.y)); }
						else if (camera.moveWithCharacter) { camera.move(sf::Vector2f(offsetX, offsetY)); }
					}
					// Missile collision check
					if (game.unit[i].hasRef(REF_UNIT_MISSILE))
					{
						for (int y = 0; y < game.unitCounter; y++)
						{
							if (i != y && math.getDistance(&game.unit[i], &game.unit[y])
								<= game.getUnitCollisionDistance(&game.unit[i], &game.unit[y])
								&& game.unit[y].type != "player")
							{
								game.removeUnit(game.unit[i].globalId);
								y = game.unitCounter;
								i -= 1;
							}
						}
					}
				}
				// Unit pickup order
				else if (game.unit[i].order[0].type == ORDER_PICKUP)
				{
					cUnit* target = &game.getUnit(game.unit[i].order[0].targetObject);
					for (int y = 0; y < target->container.itemCounter; y++) {
						game.unit[i].container.add(target->container.item[y], target->container.amount[y]);
					}
					if (core.serverMode || core.localServer) {
						game.removeUnit(game.unit[i].order[0].targetObject);
					}
					game.unit[i].removeOrder(0);
					ui.updateInterfaceItemList();
				}
				// Resource harvest
				else if (game.unit[i].order[0].type == ORDER_HARVEST)
				{
					float resFound = math.round(game.getUnit(game.unit[i].order[0].targetObject).resource * 0.20f);
					cUnit* target = &game.getUnit(game.unit[i].order[0].targetObject);
					if (core.serverMode || core.localServer) {
						game.unit[i].addResource(resFound);
						game.unit[game.getUnitId(game.unit[i].order[0].targetObject)].addResource(-resFound);
					}
					game.unit[i].removeOrder(0);
				}
				game.unit[i].updateFacing();
				game.unit[i].updateAction();
				game.unit[i].updateAnimation();
			}
		}
		// No orders
		else
		{
			// Projectile destruction
			if (game.unit[i].hasRef(REF_UNIT_MISSILE))
			{
				game.removeUnit(game.unit[i].globalId);
				i -= 1;
			}
		}
	}
	game.access.unlock();
}

void serverWorldAnim(int elapsedTime)
{
	game.access.lock();
	float timevar = (float)elapsedTime / 1000;
	timevar *= core.timeModifier;
	// Unit animations
	for (int i = 0; i < game.unitCounter; i++)
	{
		cAnimDirection animDir = game.unit[i].getCurrentAnimDirection();
		// Sprite animation
		game.unit[i].anim.curFrameTimer += timevar;
		if (game.unit[i].anim.curFrameTimer >= animDir.frameDelay)
		{
			game.unit[i].anim.curFrameTimer = 0.00f;
			game.unit[i].anim.curFrame += 1;
			if (game.unit[i].anim.curFrame >= animDir.frameCount)
			{
				game.unit[i].anim.curFrame = 0;
			}
		}
		// Health bar animation
		if (game.unit[i].displayHealth != game.unit[i].health)
		{
			if (game.unit[i].displayHealth < game.unit[i].health)
			{
				game.unit[i].displayHealth += timevar * 100.00f;
				if (game.unit[i].displayHealth > game.unit[i].health) { game.unit[i].displayHealth = game.unit[i].health; }
			}
			else if (game.unit[i].displayHealth > game.unit[i].health)
			{
				game.unit[i].displayHealth -= timevar * 100.00f;
				if (game.unit[i].displayHealth < game.unit[i].health) { game.unit[i].displayHealth = game.unit[i].health; }
			}
		}
		// Light source animation
		if (game.unit[i].light.power > 0.00f && game.unit[i].light.flickerMod != 0.00f)
		{
			game.unit[i].light.flickerCurTime += timevar * math.randf(0.50f, 1.50f);
			if (game.unit[i].light.flickerCurTime >= game.unit[i].light.flickerTime * 2.00f)
			{
				game.unit[i].light.flickerCurTime -= game.unit[i].light.flickerTime * 2.00f;
			}
		}
	}
	// World anim
	game.timeOfDay += timevar / 24.00f;
	if (game.timeOfDay >= 24.00f) { game.timeOfDay = 0.00f; }
	if (game.lightens() && game.ambientLight < game.ambientLightMax)
	{
		game.ambientLight += timevar * 3;
		if (game.ambientLight > game.ambientLightMax) { game.ambientLight = game.ambientLightMax; }
	}
	else if (game.darkens() && game.ambientLight > game.ambientLightMin)
	{
		game.ambientLight -= timevar * 3;
		if (game.ambientLight < game.ambientLightMin) { game.ambientLight = game.ambientLightMin; }
	}
	game.access.unlock();
}

void serverWorldUI(int elapsedTime)
{
	int id = -1;
	int hoverHashSum = 0;
	bool bindToMouse = false;
	bool isHovered = false;

	float timevar = (float)elapsedTime / 1000;
	timevar *= core.timeModifier;

	//sf::Vector2i mousePos = sf::Mouse::getPosition(window.winHandle);
	sf::Vector2f mousePos = window.getMousePos();

	ui.mouseNotMoving += timevar;
	ui.updateInterface();

	// Click timer
	if (ui.clickTimer > 0.00f)
	{
		ui.clickTimer -= timevar;
		if (ui.clickTimer < 0.00f) { ui.clickTimer = 0.00f; }
	}
	
	// Tooltip timers
	for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
	{
		if (ui.element[i].isValid)
		{
			isHovered = false;
			// Regular buttons
			if (ui.element[i].hasRef(REF_UI_INVENTORY_BUTTON) && sf::Keyboard::isKeyPressed(settings.hkInventory))
			{
				isHovered = true;
			}
			// Active buttons
			for (int y = 1; y < LIMIT_ACTIVEBUTTONS; y++)
			{
				if (ui.element[i].hasRef(REF_UI_ACTIVEITEM + y) && sf::Keyboard::isKeyPressed(settings.hkActiveItem[y]))
				{
					isHovered = true;
				}
			}
			if (!isHovered)
			{
				if (ui.element[i].ignoreOrigin)
				{
					isHovered = math.isPointInRect(mousePos.x, mousePos.y, ui.element[i].pos.x,
						ui.element[i].pos.y, ui.element[i].size.x, ui.element[i].size.y);
				}
				else
				{
					isHovered = math.isPointInRect(mousePos.x, mousePos.y, ui.element[i].pos.x - ui.element[i].size.x / 2,
						ui.element[i].pos.y - ui.element[i].size.y / 2, ui.element[i].size.x, ui.element[i].size.y);
				}	
			}

			// Tooltips
			if (ui.element[i].tooltip.hasText())
			{
				if (isHovered && ui.mouseNotMoving > ui.element[i].tooltip.delay && ui.element[i].tooltip.alpha <= 255)
				{
					// Update position
					if (!bindToMouse)
					{
						ui.element[i].tooltip.pos = ui.element[i].pos + ui.element[i].tooltip.offset;
					}
					else if (ui.element[i].tooltip.delay <= 0.00 || ui.element[i].tooltip.alpha == 0.00f)  {
						ui.element[i].tooltip.pos = sf::Vector2f(mousePos.x, mousePos.y);
					}
					// Add alpha
					ui.element[i].tooltip.alpha += timevar * 255 * 8;
					if (ui.element[i].tooltip.alpha > 255) { ui.element[i].tooltip.alpha = 255; }
				}
				else if (ui.element[i].tooltip.alpha > 0)
				{
					// Update position
					if (bindToMouse && ui.element[i].tooltip.delay <= 0.00)  {
						ui.element[i].tooltip.pos = sf::Vector2f(mousePos.x, mousePos.y);
					}
					// Remove alpha
					ui.element[i].tooltip.alpha -= timevar * 255 * 8;
					if (ui.element[i].tooltip.alpha < 0) { ui.element[i].tooltip.alpha = 0; }
				}
			}
		}
		// Hover
		if (ui.contextMenuTarget == CONTEXTMENU_NOTHING || ui.element[i].ref[REF_UI_CONTEXTMENU])
		{
			ui.element[i].isHovered = isHovered;
			if (isHovered) { hoverHashSum += ui.element[i].globalId; }
			if (ui.element[i].textureHovered != -1)
			{
				if (isHovered) { ui.element[i].hoverAlpha = 255; }
				else if (ui.element[i].hoverAlpha > 0.00f)
				{
					ui.element[i].hoverAlpha -= timevar * 255 * 2;
					if (ui.element[i].hoverAlpha < 0.00f) { ui.element[i].hoverAlpha = 0.00f; }
				}
			}
		}
	}
	// Hover with context menu
	if (ui.contextMenuTarget != CONTEXTMENU_NOTHING)
	{
		for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
		{
			if (ui.element[i].isValid && ui.element[i].button.args[0] == to_string(ui.contextMenuTargetId))
			{
				//ui.element[i].hoverAlpha = 255;
				i = LIMIT_UI_ELEMENTS;
			}
		}
	}
	// Hover update
	if (hoverHashSum != ui.hoverHashSum)
	{
		ui.updateInterfaceItemDescr();
	}
	ui.hoverHashSum = hoverHashSum;
	// Inventory bg alpha
	if (ui.invOpened)
	{
		id = ui.findByRef(REF_UI_INVENTORY_FULL_BG);
		if (id != -1) {
			ui.element[id].alpha = 100.00f * game.ambientLight / 255.00f + 100.00f;
		}
	}
	// Camera move
	camera.moveUpdateTimer -= timevar;
	if (camera.moveUpdateTimer < 0.00f) { camera.moveUpdateTimer = 0.00f; }
	float camSpeed = 1000.00f / camera.zoomFactor;
	float camVecModIn = 2.00f;
	float camVecModOut = 5.00f;
	//camera.moveVector = vec2(0.00f, 0.00f);
	if (GetForegroundWindow() == window.winHandle.getSystemHandle())
	{
		bool isMoving = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(settings.hkCamMove[2])
			|| window.getMousePos().x < 1.00f)
		{
			camera.move(sf::Vector2f(-camSpeed * timevar / core.timeModifier, 0.00f));
			camera.moveVector.x -= 1.00f * timevar * camVecModIn;
			if (camera.moveVector.x < -1.00f) { camera.moveVector.x = -1.00f; }
			isMoving = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(settings.hkCamMove[3])
			|| window.getMousePos().x > camera.res.x - 2.00f)
		{
			camera.move(sf::Vector2f(camSpeed * timevar / core.timeModifier, 0.00f));
			camera.moveVector.x += 1.00f * timevar * camVecModIn;
			if (camera.moveVector.x > 1.00f) { camera.moveVector.x = 1.00f; }
			isMoving = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(settings.hkCamMove[0])
			|| window.getMousePos().y < 1.00f)
		{
			camera.move(sf::Vector2f(0.00f, -camSpeed * timevar / core.timeModifier));
			camera.moveVector.y += 1.00f * timevar * camVecModIn;
			if (camera.moveVector.y > 1.00f) { camera.moveVector.y = 1.00f; }
			isMoving = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(settings.hkCamMove[1])
			|| window.getMousePos().y > camera.res.y - 2.00f)
		{
			camera.move(sf::Vector2f(0.00f, camSpeed * timevar / core.timeModifier));
			camera.moveVector.y -= 1.00f * timevar * camVecModIn;
			if (camera.moveVector.y < -1.00f) { camera.moveVector.y = -1.00f; }
			isMoving = true;
		}
		if (!isMoving)
		{
			if (camera.moveVector.x > 0.00f) {
				camera.moveVector.x -= 1.00f * timevar * camVecModOut;
				if (camera.moveVector.x < 0.00f) { camera.moveVector.x = 0.00f; }
			}
			if (camera.moveVector.x < 0.00f) {
				camera.moveVector.x += 1.00f * timevar * camVecModOut;
				if (camera.moveVector.x > 0.00f) { camera.moveVector.x = 0.00f; }
			}
			if (camera.moveVector.y > 0.00f) {
				camera.moveVector.y -= 1.00f * timevar * camVecModOut;
				if (camera.moveVector.y < 0.00f) { camera.moveVector.y = 0.00f; }
			}
			if (camera.moveVector.y < 0.00f) {
				camera.moveVector.y += 1.00f * timevar * camVecModOut;
				if (camera.moveVector.y > 0.00f) { camera.moveVector.y = 0.00f; }
			}
		}
	}
}

void serverWorldMain()
{
	int threadId = 1;
	cout << "[SRV_WORLD] Starting the server world thread" << "\n";
	srand(time(0));
	int elapsedTime, globalTime = 0;
	while (!core.shutdown)
	{
		elapsedTime = timeGetTime() - globalTime;
		globalTime = timeGetTime();
		// Ignore system time change or huge lags
		if (elapsedTime > 0 && elapsedTime < 100)
		{
			serverWorldOrders(elapsedTime);
			serverWorldAnim(elapsedTime);
			serverWorldUI(elapsedTime);
			core.thread_serverWorldTicks += 1;
			core.thread_antifreeze[threadId] = 0;
		}
		Sleep(1);
	}

	cout << "[SRV_WORLD] Cleaning up" << "\n";
}