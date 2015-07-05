
#include "main.h"
#include "server.h"
#include "settings.h"
#include "camera.h"
#include "UI.h"
#include "window.h"
#include "visual.h"
#include "client.h"
#include "math.h"
#include "path.h"

void serverWorldOrders(int elapsedTime)
{
	bool unitRemoved = false;
	float timevar = (float)elapsedTime / 1000;
	timevar *= core.timeModifier;
	float angle, offsetX, offsetY;
	for (int i = 0; i < game.unitCounter; i++)
	{
		unitRemoved = false;
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
					game.access.lock();
					// Calculating the movement speed
					float moveSpeed = game.unit[i].movementSpeed;
					angle = math.convertAngle(math.getAngle(game.unit[i].pos, game.unit[i].order[0].targetPos));
					moveSpeed *= abs(cos((game.unit[i].facingAngle - angle) / 2.00f * math.DEGTORAD));
					// Calculating the point
					if (math.getDistance(game.unit[i].pos.x, game.unit[i].pos.y, game.unit[i].order[0].targetPos.x, game.unit[i].order[0].targetPos.y)
						> moveSpeed * timevar)
					{
						angle = math.getAngle(game.unit[i].pos.x, game.unit[i].pos.y, game.unit[i].order[0].targetPos.x, game.unit[i].order[0].targetPos.y);
						offsetX = moveSpeed * timevar * cos(angle * math.DEGTORAD);
						offsetY = moveSpeed * timevar * sin(angle * math.DEGTORAD);
					}
					else
					{
						offsetX = game.unit[i].order[0].targetPos.x - game.unit[i].pos.x;
						offsetY = game.unit[i].order[0].targetPos.y - game.unit[i].pos.y;
						game.unit[i].removeOrder(0);
					}
					// Moving the unit to correct position
						// Check unit ref
					if (game.unit[i].hasRef(REF_UNIT_BESTPATHING))
					{
						// Validate the point before moving
						vec2f newPoint = path.validatePoint(vec2f(game.unit[i].pos.x + offsetX, game.unit[i].pos.y + offsetY), game.unit[i].collisionDistance, game.unit[i].globalId);
						game.unit[i].pos = newPoint;
					}
					else
					{
						// Just move the unit
						game.unit[i].pos.x += offsetX;
						game.unit[i].pos.y += offsetY;
					}
					// Check the camera status
					if (game.unit[i].globalId == client.unit)
					{
						if (camera.lockedToCharacter) { camera.moveto(sf::Vector2f(game.unit[i].pos.x, game.unit[i].pos.y)); }
						else if (camera.moveWithCharacter) { camera.move(sf::Vector2f(offsetX, offsetY)); }
					}
					game.access.unlock();
					// Missile collision check
					if (game.unit[i].hasRef(REF_UNIT_MISSILE))
					{
						for (int y = 0; y < game.unitCounter; y++)
						{
							if (i != y && math.getDistance(&game.unit[i], &game.unit[y])
								<= game.getUnitCollisionDistance(&game.unit[i], &game.unit[y])
								&& game.unit[i].owner != game.unit[y].globalId)
							{
								if (game.unit[i].hasRef(REF_UNIT_DAMAGE_RIFLE)) { game.damageUnit(game.unit[y].globalId, value.rifleDamage); }

								unitRemoved = true;
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
					if (core.serverMode || core.localServer) {
						for (int y = 0; y < target->container.itemCounter; y++) {
							game.unit[i].addItem(target->container.item[y].type, target->container.amount[y]);
						}
						game.removeUnit(game.unit[i].order[0].targetObject);
					}
					game.unit[i].removeOrder(0);
					ui.updateInterfaceItemList();
				}
				// Resource harvest
				else if (game.unit[i].order[0].type == ORDER_HARVEST)
				{
					if (core.serverSide())
					{
						float resFound;
						cUnit* target = &game.getUnit(game.unit[i].order[0].targetObject);
						// If at least some tool is used
						if (game.unit[i].order[0].paramA > 0)
						{
							resFound = game.getUnit(game.unit[i].order[0].targetObject).resource;
							game.unit[i].addResource(resFound);
							game.unit[game.getUnitId(game.unit[i].order[0].targetObject)].addOrder_death();
							game.unit[game.getUnitId(game.unit[i].order[0].targetObject)].order[0].paramA = game.unit[i].order[0].paramA;
						}
						// No tools used
						else
						{
							resFound = math.round(game.getUnit(game.unit[i].order[0].targetObject).resource);
							if (resFound == 275) { resFound = 75; }
							else if (resFound == 200) { resFound = 50; }
							else if (resFound == 150) { resFound = 30; }
							else if (resFound == 120) { resFound = 25; }
							else if (resFound == 95) { resFound = 20; }
							else if (resFound == 75) { resFound = 15; }
							else if (resFound >= 10) { resFound = 10; }
							else { resFound = 0; }
							game.unit[i].addResource(resFound);
							game.unit[game.getUnitId(game.unit[i].order[0].targetObject)].addResource(-resFound);
						}
					}
					game.unit[i].removeOrder(0);
				}
				// Death
				else if (game.unit[i].order[0].type == ORDER_DEATH && core.serverSide())
				{
					int usedPowerLevel = game.unit[i].order[0].paramA;
					// Dropping container items
					if (game.unit[i].container.itemCounter > 0 || game.unit[i].drop.itemCounter > 0)
					{
						int itemId = game.addUnit("item_a", game.getUnit(game.unit[i].globalId).pos);
						cItemContainer cont = game.getUnit(game.unit[i].globalId).container;
						for (int a = 0; a < cont.itemCounter; a++)
						{
							game.getUnit(itemId).addItem(cont.item[a].type, cont.amount[a]);
						}
						game.unit[i].drop.flush(itemId, usedPowerLevel);
						// If nothing dropped, remove the unit
						if (game.getUnit(itemId).container.itemCounter == 0) {
							game.removeUnit(itemId);
						}
					}
					// Removing the unit
					unitRemoved = true;
					game.removeUnit(game.unit[i].globalId);
					i -= 1;
				}

				// If unit is not removed, update it
				if (!unitRemoved)
				{
					game.unit[i].updateFacing();
					game.unit[i].updateAction();
					game.unit[i].updateAnimation();
				}
			}
		}
		// No orders
		else
		{
			// Projectile destruction
			if (game.unit[i].hasRef(REF_UNIT_MISSILE))
			{
				unitRemoved = true;
				game.removeUnit(game.unit[i].globalId);
				i -= 1;
			}
		}
		// Life timer
		if (game.unit[i].lifeTimer.enabled)
		{
			game.unit[i].lifeTimer.time -= timevar;
			// Your time has come...
			if (game.unit[i].lifeTimer.time <= 0.00f)
			{
				unitRemoved = true;
				game.removeUnit(game.unit[i].globalId);
				i -= 1;
			}
		}
	}
}

void serverWorldAnim(int elapsedTime)
{
	//game.access.lock();
	mutex.renderUnits.lock();
	float timevar = (float)elapsedTime / 1000;
	timevar *= core.timeModifier;
	// Unit animations
	for (int i = 0; i < game.unitCounter; i++)
	{
		cAnimDirection animDir = game.unit[i].getCurrentAnimDirection().data;
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
	mutex.renderUnits.unlock();
	//game.access.unlock();

	// World anim
	game.timeOfDay += timevar * settings.wdDayNightSpeed;
	if (game.timeOfDay >= 24.00f) { game.timeOfDay = 0.00f; }
	if (game.lightens() && game.ambientLight < game.ambientLightMax)
	{
		game.ambientLight += timevar * settings.wdNightChangeSpeed;
		if (game.ambientLight > game.ambientLightMax) { game.ambientLight = game.ambientLightMax; }
	}
	else if (game.darkens() && game.ambientLight > game.ambientLightMin)
	{
		game.ambientLight -= timevar * settings.wdNightChangeSpeed;
		if (game.ambientLight < game.ambientLightMin) { game.ambientLight = game.ambientLightMin; }
	}

	// Hovered unit
	mutex.renderMain.lock();
	int oldHover = visual.hoveredUnit;
	int oldPriority = -1;
	visual.hoveredUnit = -1;
	vec2f mousePos = window.getMousePos(true);
	for (int i = 0; i < game.unitCounter; i++)
	{
		if (!game.unit[i].hasRef(REF_UNIT_NOSELECTION)
			&& util.intersects(mousePos, game.unit[i].pos - game.unit[i].center, game.unit[i].size)
			&& (visual.hoveredUnit == -1 || game.unit[i].globalId == oldHover || game.unit[i].selectionPriority > oldPriority))
		{
			visual.hoveredUnit = game.unit[i].globalId;
			oldPriority = game.unit[i].selectionPriority;
		}
	}
	mutex.renderMain.unlock();
}

void serverWorldUI(int elapsedTime)
{
	int id = -1;
	int hoverHashSum = 0;
	bool bindToMouse = math.intToBool(settings.enableDynamicTooltips);
	bool isHovered = false;
	sf::Packet data;

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
						ui.element[i].tooltip.pos = sf::Vector2f(mousePos.x, mousePos.y) + ui.element[i].tooltip.offset;
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
	if (GetForegroundWindow() == window.winHandle.getSystemHandle() && !console.displayed)
	{
		bool isMoving = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(settings.hkCamMove[2])
			|| (settings.enableMouseScroll && window.getMousePos().x < 1.00f))
		{
			camera.move(sf::Vector2f(-camSpeed * timevar / core.timeModifier, 0.00f));
			camera.moveVector.x -= 1.00f * timevar * camVecModIn;
			if (camera.moveVector.x < -1.00f) { camera.moveVector.x = -1.00f; }
			isMoving = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(settings.hkCamMove[3])
			|| (settings.enableMouseScroll && window.getMousePos().x > camera.res.x - 2.00f))
		{
			camera.move(sf::Vector2f(camSpeed * timevar / core.timeModifier, 0.00f));
			camera.moveVector.x += 1.00f * timevar * camVecModIn;
			if (camera.moveVector.x > 1.00f) { camera.moveVector.x = 1.00f; }
			isMoving = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(settings.hkCamMove[0])
			|| (settings.enableMouseScroll && window.getMousePos().y < 1.00f))
		{
			camera.move(sf::Vector2f(0.00f, -camSpeed * timevar / core.timeModifier));
			camera.moveVector.y += 1.00f * timevar * camVecModIn;
			if (camera.moveVector.y > 1.00f) { camera.moveVector.y = 1.00f; }
			isMoving = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(settings.hkCamMove[1])
			|| (settings.enableMouseScroll && window.getMousePos().y > camera.res.y - 2.00f))
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

void serverWorldUnits(int elapsedTime)
{
	float timevar = (float)elapsedTime / 1000;
	timevar *= core.timeModifier;

	for (int i = 0; i < game.unitCounter; i++)
	{
		// Checking rotation
		if (abs(game.unit[i].facingAngle - game.unit[i].targetFacingAngle) > 0.50f) {
			float realAngle = game.unit[i].facingAngle;
			float targetAngle = game.unit[i].targetFacingAngle;
			//if (abs(targetAngle - realAngle) >) { realAngle += 360.00f; }
			//console.debug << "[DEBUG] Current: " << game.unit[i].facingAngle << ", Target: " << game.unit[i].targetFacingAngle << ", Speed: " << game.unit[i].rotateSpeed << endl;
			if (abs(realAngle - targetAngle) <= game.unit[i].rotateSpeed * timevar) {
				game.unit[i].facingAngle = game.unit[i].targetFacingAngle;
			}
			else {
				if (targetAngle - realAngle > 180.00f) { game.unit[i].facingAngle -= game.unit[i].rotateSpeed * timevar; }
				else if (realAngle < targetAngle || realAngle - targetAngle > 180.00f) { game.unit[i].facingAngle += game.unit[i].rotateSpeed * timevar; }
				else { game.unit[i].facingAngle -= game.unit[i].rotateSpeed * timevar; }
			}
			// No overshoot
			if (game.unit[i].facingAngle > 360.00f) { game.unit[i].facingAngle -= 360.00f; }
			else if (game.unit[i].facingAngle < 0.00f) { game.unit[i].facingAngle += 360.00f; }
		}
		// Checking buffs
		for (int y = 0; y < (int)game.unit[i].buff.list.size(); y++)
		{
			if (game.unit[i].buff.list[y].duration > 0.00f) {
				game.unit[i].buff.list[y].duration -= timevar;
				if (game.unit[i].buff.list[y].duration <= 0.00f) {
					game.unit[i].buff.removeById(y);
				}
			}
		}
		// Checking max health
		if (game.unit[i].health > game.unit[i].maxHealth) { game.unit[i].health = game.unit[i].maxHealth; }
		// Checking for death
		if (game.unit[i].maxHealth > 0.00 && game.unit[i].health <= 0.00f && game.unit[i].order[0].type != ORDER_DEATH)
		{
			game.killUnit(game.unit[i].globalId);
		}
	}
}

void serverWorldConnection(int elapsedTime)
{
	sf::Packet data;
	// Ping
	client.pingTimer += elapsedTime;
	server.pingTimer += elapsedTime;
	
	// Sending the mouse position
	vec2f mousePos = window.getMousePos(true);
	if (client.connected && mousePos != client.lastMousePos && client.mousePosTimer >= 10)
	{
		client.mousePosTimer = 0;
		client.lastMousePos = mousePos;
		// Sending mouse position
		data << MSG_INFO_MOUSEPOS << mousePos.x << mousePos.y;
		client.sendPacket(data);
		data.clear();
	}
	else if (client.mousePosTimer < 10) {
		client.mousePosTimer += elapsedTime;
	}

	// Mouse dragging move order
	if (client.connected && mousePos != client.lastMoveOrderPos && client.moveOrderTimer >= 50 && ui.mouseStateLMB == MOUSE_CONTROLS_MOVEMENT)
	{
		client.moveOrderTimer = 0;
		client.lastMoveOrderPos = mousePos;
		// Sending the packet
		data << MSG_CONTROLS_MOVETO << mousePos.x << mousePos.y << false;
		client.sendPacket(data);
		data.clear();
	}
	else if (client.mousePosTimer < 50) {
		client.moveOrderTimer += elapsedTime;
	}
}

void serverWorldMain()
{
	int threadId = 1;
	console << "[SRV_WORLD] Starting the server world thread" << "\n";
	srand(time(0));
	int elapsedTime, globalTime = 0;
	while (!core.thread_shutdown[threadId])
	{
		elapsedTime = timeGetTime() - globalTime;
		globalTime = timeGetTime();
		// Ignore system time change or huge lags
		if (elapsedTime > 0 && elapsedTime < 100)
		{
			serverWorldOrders(elapsedTime);
			serverWorldAnim(elapsedTime);
			serverWorldUI(elapsedTime);
			serverWorldUnits(elapsedTime);
			serverWorldConnection(elapsedTime);
			core.thread_serverWorldTicks += 1;
			core.thread_antifreeze[threadId] = 0;
		}
		Sleep(3);
	}

	console << "[SRV_WORLD] Cleaning up" << "\n";
}