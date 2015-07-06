
#include "main.h"
#include "window.h"
#include "game.h"
#include "settings.h"
#include "UI.h"
#include "database.h"
#include "camera.h"
#include "visual.h"
#include "math.h"
#include "editor.h"
#include "client.h"
#include "target.h"

sf::RectangleShape brushRect;
sf::CircleShape brushCircle;
sf::Vertex vertex[4];
sf::VertexArray brushVertex;
sf::Text brushText;

void cWindow::mainPaint()
{
	// Clearing the window
	window.winHandle.clear(sf::Color(127, 127, 127));
	window.texHandle.clear(sf::Color(127, 127, 127));
	window.texHandleShadow.clear(sf::Color(255, 255, 255));
	window.texHandleLight.clear(sf::Color(min(255.00f, game.ambientLight), min(255.00f, game.ambientLight), min(255.00f, game.ambientLight)));
	window.texHandleLightMult.clear(sf::Color(0, 0, 0, 0));
	window.texHandleTop.clear(sf::Color(127, 127, 127));
	// Initializing the brushes
	brushText.setFont(visual.fontMain);
	// Loading the matrix
	float prec = 1.00f;
	mutex.mainMatrix.lock();
	window.matrixHandle = sf::Transform::Identity;
	window.matrixHandle.scale(vec2(settings.sampleMod, settings.sampleMod));
	window.matrixHandle.scale(vec2(camera.zoomFactor, camera.zoomFactor), vec2(camera.res.x / 2, camera.res.y / 2));
	window.matrixHandle.translate(-camera.pos);
	mutex.mainMatrix.unlock();
	//window.matrixHandle.translate(sf::Vector2f(math.round(-camera.pos.x * prec) / prec, math.round(-camera.pos.y * prec) / prec));
	// Painting the game data
	if (!core.serverMode)
	{
		mutex.renderMain.lock();
		if (core.menuState == STATE_GAME)
		{
			//game.access.lock();
			window.paintTileMap();
			window.paintUnits();
			//game.access.unlock();
			window.paintLighting();
			window.paintPostFX();
		}
		window.paintUI();
		window.paintDebugInfo();
		mutex.renderMain.unlock();
	}
	window.paintConsole();
	sf::Sprite buffer;

	// Flushing the buffer
	window.texHandleTop.display();
	if (settings.enableTextureSmoothing) { window.texHandleTop.setSmooth(true); }
	else { window.texHandleTop.setSmooth(false); }
	buffer = sf::Sprite(window.texHandleTop.getTexture());
	buffer.scale(1.00f / settings.sampleMod, 1.00f / settings.sampleMod);

	//window.texHandleLight.display();
	//buffer = sf::Sprite(window.texHandleLight.getTexture());
	//window.texHandleShadow.display();
	//buffer = sf::Sprite(window.texHandleShadow.getTexture());
	window.winHandle.draw(buffer);
	window.winHandle.display();
	core.thread_windowTicks += 1;
}

cAnimDisplay animDisplay;
void cWindow::paintUnits()
{
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

	brushVertex.setPrimitiveType(sf::PrimitiveType::Quads);
	brushRect.setOutlineColor(sf::Color(0, 255, 0));

	game.access.lock();
	visual.unitsPainted = 0;
	int repeats = 1;
	if (settings.enableBetterShadows) { repeats += 1; }
	mutex.renderUnits.lock();
	for (int u = 0; u < repeats; u++)
	{
		for (float y = cameraTop; y < cameraBot; y += step)
		{
			for (int i = 0; i < game.unitCounter; i++)
			{
				unitLeft = game.unit[i].pos.x - game.unit[i].center.x;
				unitRight = unitLeft + game.unit[i].size.x;

				if (!game.unit[i].hasRef(REF_UNIT_NORENDER)
					&& ((game.unit[i].pos.y >= y && game.unit[i].pos.y < y + step && unitRight >= cameraLeft && unitLeft <= cameraRight && !game.unit[i].hasRef(REF_UNIT_ALWAYSVISIBLE))
					|| (game.unit[i].hasRef(REF_UNIT_ALWAYSVISIBLE) && y == cameraTop) )) 
				{
					// Display the selection circle
					/*if (visual.hoveredUnit == game.unit[i].globalId)
					{
						brushRect.setRotation(0.00f);
						brushRect.setScale(1.00f, 1.00f);
						brushRect.setFillColor(sf::Color(255, 127, 0));
						brushRect.setPosition(game.unit[i].pos + game.unit[i].selectionOffset);
						brushRect.setOrigin(game.unit[i].interactDistance, game.unit[i].interactDistance);
						brushRect.setSize(vec2f(game.unit[i].interactDistance * 2, game.unit[i].interactDistance * 2));
						brushRect.setTexture(&visual.gameTex[database.texture[TEX_SELECTION_CIRCLE]].handle, true);

						// Last-minute check
						if (!settings.enableBetterShadows) { window.texHandle.draw(brushRect, window.matrixHandle); }
						else
						{
							if (u == 0) { window.texHandle.draw(brushRect, window.matrixHandle); }
							else { window.texHandleShadow.draw(brushRect, window.matrixHandle); }
						}
					}*/

					// Unit animation
					animDisplay = game.unit[i].getCurrentAnimDirection();

					int anim = game.unit[i].anim.type;
					int curFrame = game.unit[i].anim.curFrame;
					int texWidth, texHeight, frameWidth;

					texWidth = visual.gameTex[animDisplay.data.tex].handle.getSize().x;
					texHeight = visual.gameTex[animDisplay.data.tex].handle.getSize().y;
					frameWidth = texWidth / animDisplay.data.frameCount;

					brushRect.setTexture(&visual.gameTex[animDisplay.data.tex].handle);
					if (animDisplay.texMod == 1) {
						brushRect.setTextureRect(sf::IntRect(curFrame * frameWidth, 0, frameWidth, texHeight));
					}
					else {
						brushRect.setTextureRect(sf::IntRect((curFrame + 1) * frameWidth, 0, -frameWidth, texHeight));
					}

					// Shadow (day)
					if (!game.unit[i].hasRef(REF_UNIT_NOSHADOW) && (u == 1 || !settings.enableBetterShadows))
					{
						float shadowAngleMod = 4.00f;
						float shadowScaleMod = 1.00f;
						float shadowScaleMin = 1.20f;
						float shadowAngle = 0.00f, shadowScale = 1.20f;

						float timeLocal = game.timeOfDay;
						float shadowBrightness = (game.ambientLight - 100.00f) / 255.00f * 210.00f;
						shadowBrightness = max(0.00f, min(255.00f, shadowBrightness));
						if (shadowBrightness > 0.00f)
						{
							shadowScale = abs(timeLocal - 12.00f) / 6.00f * shadowScaleMod + shadowScaleMin;
							shadowAngle = (timeLocal - 12.00f) * shadowAngleMod;

							visual.shadowBrightness = shadowBrightness;
							brushRect.setFillColor(sf::Color(0, 0, 0, shadowBrightness));
							if (settings.enableBetterShadows) {
								brushRect.setFillColor(sf::Color(0, 0, 0, 255.00f));
							}
							//brushRect.setPosition(game.unit[i].pos.x + game.unit[i].shadowOffset.x,
												  //game.unit[i].pos.y + game.unit[i].shadowOffset.y);
							vec2 newPos = math.polar(game.unit[i].pos + game.unit[i].shadowOffset, game.unit[i].flyingHeight * shadowScale,
								shadowAngle * game.unit[i].flyingHeight / 10.00f - 90.00f);
							brushRect.setPosition(newPos);
							brushRect.setOrigin(game.unit[i].center.x, game.unit[i].center.y);
							brushRect.setSize(sf::Vector2f(game.unit[i].size.x, game.unit[i].size.y));
							brushRect.setScale(1.00f, shadowScale);
							brushRect.setRotation(shadowAngle);
							// Rotatable unit
							if (game.unit[i].hasRef(REF_UNIT_ROTATE)) {
								brushRect.setRotation(math.convertAngle(shadowAngle) - game.unit[i].facingAngle);
								brushRect.setScale(1.00f, 1.00f);
							}
							// Shadow texture override
							if (animDisplay.data.texShadow != -1) {
								brushRect.setTexture(&visual.gameTex[animDisplay.data.texShadow].handle);
							}

							// Last-minute check
							objRect = brushRect.getGlobalBounds();
							if (camRect.intersects(objRect))
							{
								renderState.transform = window.matrixHandle;
								if (visual.shader[SHADER_SHADOW].isAvailable() && settings.shadowBlur > 1)
								{
									visual.shader[SHADER_SHADOW].setParameter("transpar", shadowBrightness / 255.00f);
									visual.shader[SHADER_SHADOW].setParameter("sampleCount", settings.shadowBlur);
									renderState.shader = &visual.shader[SHADER_SHADOW];
								}
								if (!settings.enableBetterShadows) { window.texHandle.draw(brushRect, renderState); }
								else { window.texHandleShadow.draw(brushRect, renderState); }
							}
						}

						// Shadow (night)
						//visual.updateLightLevel(game.unit[i].pos);
						shadowBrightness = -(game.ambientLight - 100.00f) / 255.00f * 210.00f * 1.50f;// - visual.maxLight / 10.00f;
						shadowBrightness = max(0.00f, min(255.00f, shadowBrightness));
						if (shadowBrightness > 0.00f && settings.enableNightShadows)
						{
							shadowAngle = 0.00f, shadowScale = 1.20f;
							if (timeLocal > 18.00f) { shadowScale = (24.00f - timeLocal) / 6.00f * shadowScaleMod + shadowScaleMin; }
							else { shadowScale = timeLocal / 6.00f * shadowScaleMod + shadowScaleMin; }

							if (timeLocal > 18.00f) { shadowAngle = -(24.00f - timeLocal) * shadowAngleMod; }
							else { shadowAngle = timeLocal * shadowAngleMod; }

							visual.shadowBrightness = shadowBrightness;
							brushRect.setFillColor(sf::Color(0, 0, 0, shadowBrightness));
							if (settings.enableBetterShadows) {
								brushRect.setFillColor(sf::Color(0, 0, 0, 255.00f));
							}
							vec2 newPos = math.polar(game.unit[i].pos + game.unit[i].shadowOffset, game.unit[i].flyingHeight * shadowScale,
								shadowAngle * game.unit[i].flyingHeight / 10.00f - 90.00f);
							brushRect.setPosition(newPos);
							brushRect.setOrigin(game.unit[i].center.x, game.unit[i].center.y);
							brushRect.setSize(sf::Vector2f(game.unit[i].size.x, game.unit[i].size.y));
							brushRect.setScale(1.00f, shadowScale);
							brushRect.setRotation(shadowAngle);
							// Rotatable unit
							if (game.unit[i].hasRef(REF_UNIT_ROTATE)) {
								brushRect.setRotation(math.convertAngle(shadowAngle) - game.unit[i].facingAngle);
								brushRect.setScale(1.00f, 1.00f);
							}
							// Shadow texture override
							if (animDisplay.data.texShadow != -1) {
								brushRect.setTexture(&visual.gameTex[animDisplay.data.texShadow].handle);
							}
							renderState.transform = window.matrixHandle;

							// Last-minute check
							objRect = brushRect.getGlobalBounds();
							if (camRect.intersects(objRect))
							{
								if (visual.shader[SHADER_SHADOW].isAvailable() && settings.shadowBlur > 1)
								{
									visual.shader[SHADER_SHADOW].setParameter("transpar", shadowBrightness / 255.00f);
									visual.shader[SHADER_SHADOW].setParameter("sampleCount", settings.shadowBlur);
									renderState.shader = &visual.shader[SHADER_SHADOW];
								}
								if (!settings.enableBetterShadows) { window.texHandle.draw(brushRect, renderState); }
								else { window.texHandleShadow.draw(brushRect, renderState); }
							}
						}
					}

					// Display the object
					brushRect.setRotation(0.00f);
					brushRect.setScale(1.00f, 1.00f);
					brushRect.setFillColor(sf::Color(255, 255, 255));
					// Hover
					if (visual.hoveredUnit == game.unit[i].globalId) {
						brushRect.setFillColor(settings.visualUnitHoverColor);
					}
					// No light
					if (!settings.enableDynamicLight) { brushRect.setFillColor(sf::Color(
							min(game.ambientLight, 255.0f),
							min(game.ambientLight, 255.0f),
							min(game.ambientLight, 255.0f)));
					}
					// Placeholder for building
					if (game.unit[i].hasRef(REF_UNIT_PLACEHOLDER) && target.activeBuild) {
						if (target.isBuildGood) { brushRect.setFillColor(sf::Color(0, 255, 0, 175)); }
						else { brushRect.setFillColor(sf::Color(255, 0, 0, 175)); }
					}
					brushRect.setPosition((game.unit[i].pos.x), (game.unit[i].pos.y));
					brushRect.setOrigin(game.unit[i].center.x, game.unit[i].center.y);
					brushRect.setSize(sf::Vector2f(game.unit[i].size.x, game.unit[i].size.y));
						// Editor selection
					if (editor.sel.isSelected(game.unit[i].globalId)) { brushRect.setOutlineThickness(-1.00f); }
						// Rotatable unit
					if (game.unit[i].hasRef(REF_UNIT_ROTATE)) { brushRect.setRotation(-game.unit[i].facingAngle); }
						// Shadow texture reset
					if (animDisplay.data.texShadow != -1) {
						brushRect.setTexture(&visual.gameTex[animDisplay.data.tex].handle);
					}
						// Last-minute check
					objRect = brushRect.getGlobalBounds();
					cameraIntersection = camRect.intersects(objRect);
					if (core.advancedDebug || cameraIntersection)
					{
						if (!settings.enableBetterShadows) { window.texHandle.draw(brushRect, window.matrixHandle); }
						else
						{
							if (u == 0) { window.texHandle.draw(brushRect, window.matrixHandle); }
							else { window.texHandleShadow.draw(brushRect, window.matrixHandle); }
						}
						if (u == 0) { visual.unitsPainted += 1; }
					}
					brushRect.setRotation(0.00f);
					brushRect.setOutlineThickness(0.00f);
				}
			}
		}
	}
	mutex.renderUnits.unlock();
	game.access.unlock();
}

sf::FloatRect tileRect, tileRectTex;
sf::IntRect intRectTex;
void cWindow::paintTileMap()
{
	sf::Vector2u texSize = visual.gameTex[database.texture[TEX_WORLD_GROUND]].handle.getSize();
	sf::Transform miniMatrix;
	miniMatrix.scale(sf::Vector2f(settings.sampleMod, settings.sampleMod));

	tileRect.left = 0;
	tileRect.top = 0;
	tileRect.width = camera.res.x * settings.sampleMod;
	tileRect.height = camera.res.y * settings.sampleMod;
	tileRectTex = window.matrixHandle.getInverse().transformRect(tileRect);

	brushRect.setOrigin(0, 0);
	brushRect.setSize(sf::Vector2f(camera.res.x * settings.sampleMod, camera.res.y * settings.sampleMod));
	brushRect.setTexture(&visual.gameTex[database.texture[TEX_WORLD_GROUND]].handle);
	brushRect.setFillColor(sf::Color(255, 255, 255));
	if (!settings.enableDynamicLight) { brushRect.setFillColor(sf::Color(
			min(game.ambientLight, 255.0f),
			min(game.ambientLight, 255.0f),
			min(game.ambientLight, 255.0f)));
	}
	intRectTex = sf::IntRect(tileRectTex.left, tileRectTex.top, tileRectTex.width, tileRectTex.height);
	brushRect.setPosition((float)intRectTex.left - tileRectTex.left, (float)intRectTex.top - tileRectTex.top);
	brushRect.setTextureRect(intRectTex);

	/*brushRect.setPosition(0.00f, 0.00f);
	brushRect.setTextureRect(sf::IntRect(tileRectTex));*/
	window.texHandle.draw(brushRect);
	if (settings.enableBetterShadows) { window.texHandleShadow.draw(brushRect); }
}

void cWindow::paintLighting()
{
	float power;
	int tex;
	int lightsDisplayed = 0;
	int priority = 0;

	for (int a = 0; a < 2; a++)
	{
		priority = 0;
		while (priority < LIMIT_PRIORITY_LIGHT && lightsDisplayed < game.unitCounter)
		{
			for (int i = 0; i < game.unitCounter; i++)
			{
				if (game.unit[i].light.priority == priority && game.unit[i].light.power > 0 && game.unit[i].light.texture != -1)
				{
					tex = game.unit[i].light.texture;
					brushRect.setTexture(&visual.gameTex[tex].handle);
					brushRect.setPosition(game.unit[i].pos);
					brushRect.setFillColor(sf::Color(255, 255, 255, max(0.00f, min(255.00f, 300.00f - game.ambientLight))));
					if (!settings.enableDynamicLight) {
						brushRect.setTexture(&visual.gameTex[visual.addTexture("light_white.png")].handle);
						brushRect.setFillColor(sf::Color(255, 255, 255, 50.00f));
					}
					brushRect.setTextureRect(sf::IntRect(0, 0, visual.gameTex[tex].handle.getSize().x, visual.gameTex[tex].handle.getSize().y));
					power = game.unit[i].light.power;
					if (game.unit[i].light.flickerMod != 0.00f) {
						power += power * (game.unit[i].light.flickerMod * abs(game.unit[i].light.flickerCurTime / game.unit[i].light.flickerTime - 1.00f));
						power += math.randf(-5.00f, 5.00f);
					}
					brushRect.setOrigin(sf::Vector2f(power, power));
					brushRect.setSize(sf::Vector2f(power * 2.00f, power * 2.00f));
					// Directional
					brushRect.setRotation(0.00f);
					if (game.unit[i].light.directional) {
						brushRect.setRotation(-game.unit[i].facingAngle);
					}
					// Painting to two textures
					if (settings.enableDynamicLight && a == 0) { window.texHandleLight.draw(brushRect, window.matrixHandle); }
					else if (settings.enableDynamicLight && a == 1) { window.texHandleLightMult.draw(brushRect, window.matrixHandle); }
					else { window.texHandle.draw(brushRect, window.matrixHandle); }
				}
			}
			priority += 1;
		}
		if (!math.intToBool(settings.enableBetterLight)) { break; }
	}
	brushRect.setRotation(0.00f);
	window.texHandleLight.display();
	if (settings.enableBetterLight) { window.texHandleLightMult.display(); }
}

void cWindow::paintPostFX()
{
	bool bufferReady = false;
	sf::Shader *shader;
	sf::RenderStates state;
	sf::Sprite buffer;
	
	window.texHandle.display();
	// Universal shader
	if (settings.enableScreenShaders == 1 && sf::Shader::isAvailable())
	{
		if (settings.enableBetterShadows) { window.texHandleShadow.display(); }
		buffer = sf::Sprite(window.texHandle.getTexture());
		shader = &visual.shader[SHADER_FULLSCREEN];
		float boost = game.ambientLight / 255.00f * 1.00f;
		boost = max(1.00f, boost);
		shader->setParameter("texMain", sf::Shader::CurrentTexture);
		shader->setParameter("boostR", boost);
		shader->setParameter("boostG", boost);
		shader->setParameter("boostB", boost);
		shader->setParameter("texLight", window.texHandleLight.getTexture());
		shader->setParameter("texLightMult", window.texHandleLightMult.getTexture());
		shader->setParameter("ambientLight", game.ambientLight / 255.00f);
		if (settings.enableBetterLight)
		{
			shader->setParameter("enableBetterLight", 1.00f);
		}
		else { shader->setParameter("enableBetterLight", 0.00f); }
		if (settings.enableBetterShadows)
		{
			shader->setParameter("checkForShadow", 1.00f);
			shader->setParameter("shadowBrightness", 1.00f - (visual.shadowBrightness / 255.00f));
			shader->setParameter("texShadow", window.texHandleShadow.getTexture());
		}
		else { shader->setParameter("checkForShadow", 0.00f); } 
		window.texHandleTop.draw(buffer, shader);
		bufferReady = true;
	}
	// Alternate light
	else if (settings.enableDynamicLight == 1)
	{
		buffer = sf::Sprite(window.texHandle.getTexture());
		shader = &visual.shader[SHADER_LIGHT];
		shader->setParameter("tex_main", sf::Shader::CurrentTexture);
		shader->setParameter("tex_light", window.texHandleLight.getTexture());
		bufferReady = true;
		window.texHandleTop.draw(buffer, shader);
	}
	// Camera blur
	if (settings.enableCameraBlur && (camera.moveVector.x != 0 || camera.moveVector.y != 0))
	{
		if (bufferReady)
		{
			window.texHandleTop.display();
			buffer = sf::Sprite(window.texHandleTop.getTexture());
		}
		else
		{
			window.texHandle.display();
			buffer = sf::Sprite(window.texHandle.getTexture());
		}
		shader = &visual.shader[SHADER_CAMBLUR];
		shader->setParameter("iSampleCount", 1.00f);
		shader->setParameter("sampleOffset", 0.0025f);
		shader->setParameter("camVectorX", camera.moveVector.x);
		shader->setParameter("camVectorY", camera.moveVector.y);
		window.texHandleTop.draw(buffer, shader);
		bufferReady = true;
	}
	// No shaders used
	if (!bufferReady)
	{
		buffer = sf::Sprite(window.texHandle.getTexture());
		window.texHandleTop.draw(buffer);
	}
}

void cWindow::paintUI()
{
	ui.access.lock();

	float fade;
	string text;
	sf::FloatRect floatRect;
	brushText.setCharacterSize(16);
	sf::Transform miniMatrix;
	miniMatrix.scale(sf::Vector2f(settings.sampleMod, settings.sampleMod));
	// UI Elements
	for (int y = 0; y < LIMIT_PRIORITY_UI; y++)
	{
		for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
		{
			if (ui.element[i].isValid && ui.element[i].priority == y)
			{
				fade = ui.element[i].getFade();
				// Textured body
				// Normal
				if (ui.element[i].texture != -1)
				{
					brushRect.setFillColor(sf::Color(255, 255, 255, ui.element[i].alpha * fade));
					brushRect.setOrigin(0.00f, 0.00f);
					if (!ui.element[i].ignoreOrigin) { brushRect.setOrigin(ui.element[i].size.x / 2, ui.element[i].size.y / 2); }
					brushRect.setPosition(ui.element[i].pos.x, ui.element[i].pos.y);
					brushRect.setSize(ui.element[i].size);
					brushRect.setTexture(&visual.gameTex[ui.element[i].texture].handle);
					brushRect.setTextureRect(sf::IntRect(0, 0, visual.gameTex[ui.element[i].texture].handle.getSize().x, visual.gameTex[ui.element[i].texture].handle.getSize().y));
					window.texHandleTop.draw(brushRect, miniMatrix);
				}
				// Hover
				if (ui.element[i].hoverAlpha > 0 && ui.element[i].textureHovered != -1)
				{
					brushRect.setFillColor(sf::Color(255, 255, 255, min(255.00f, (float)ui.element[i].hoverAlpha * ((float)ui.element[i].alpha / 255.00f)) * fade));
					brushRect.setOrigin(0.00f, 0.00f);
					if (!ui.element[i].ignoreOrigin) { brushRect.setOrigin(ui.element[i].size.x / 2, ui.element[i].size.y / 2); }
					brushRect.setPosition(ui.element[i].pos.x, ui.element[i].pos.y);
					brushRect.setSize(ui.element[i].size);
					brushRect.setTexture(&visual.gameTex[ui.element[i].textureHovered].handle);
					brushRect.setTextureRect(sf::IntRect(0, 0, visual.gameTex[ui.element[i].textureHovered].handle.getSize().x, visual.gameTex[ui.element[i].textureHovered].handle.getSize().y));
					window.texHandleTop.draw(brushRect, miniMatrix);
				}
				// Bar
				if (ui.element[i].bar.texture != -1)
				{
					brushRect.setFillColor(sf::Color(255, 255, 255, ui.element[i].alpha * fade));
					brushRect.setOrigin(0.00f, 0.00f);
					if (!ui.element[i].ignoreOrigin) { brushRect.setOrigin(ui.element[i].size.x / 2, ui.element[i].size.y / 2); }
					brushRect.setTexture(&visual.gameTex[ui.element[i].bar.texture].handle);

					float barValue = max(0.00f, min(1.00f, ui.element[i].bar.value / ui.element[i].bar.maxValue));
					float rawBarValue = barValue;
					barValue = barValue * (1.00f - ui.element[i].bar.maxOffset);
					barValue = barValue + ui.element[i].bar.minOffset - rawBarValue * ui.element[i].bar.minOffset;
					//barValue = barValue - barValue * ui.element[i].bar.maxOffset;
					//barValue -= ui.element[i].bar.maxOffset * barValue;
					if (ui.element[i].bar.vertical) {
						brushRect.setPosition(ui.element[i].pos.x, ui.element[i].pos.y + ui.element[i].size.y);
						brushRect.setSize(sf::Vector2f(ui.element[i].size.x, -ui.element[i].size.y * barValue));
						brushRect.setTextureRect(sf::IntRect(0,
							math.round(visual.gameTex[ui.element[i].bar.texture].handle.getSize().y * (1.00 - barValue)),
							visual.gameTex[ui.element[i].bar.texture].handle.getSize().x,
							math.round(visual.gameTex[ui.element[i].bar.texture].handle.getSize().y * barValue)));
					}
					else { 
						brushRect.setPosition(ui.element[i].pos.x, ui.element[i].pos.y);
						brushRect.setSize(sf::Vector2f(ui.element[i].size.x * barValue, ui.element[i].size.y));
						brushRect.setTextureRect(sf::IntRect(0, 0,
							math.round(visual.gameTex[ui.element[i].bar.texture].handle.getSize().x * barValue),
							visual.gameTex[ui.element[i].bar.texture].handle.getSize().y));
					}
					window.texHandleTop.draw(brushRect, miniMatrix);
				}
				// Text
				if (ui.element[i].hasText())
				{
					float textAlpha = (float)ui.element[i].hoverAlpha / 255.00f;
					if (ui.element[i].textFont == FONT_DESCR) { brushText.setFont(visual.fontDescr); }
					else { brushText.setFont(visual.fontMain); }
					brushText.setCharacterSize(ui.element[i].textSize);
					brushText.setColor(sf::Color(
						ui.element[i].textColor.r * (1.00f - textAlpha) + ui.element[i].textColorHover.r * textAlpha,
						ui.element[i].textColor.g * (1.00f - textAlpha) + ui.element[i].textColorHover.g * textAlpha,
						ui.element[i].textColor.b * (1.00f - textAlpha) + ui.element[i].textColorHover.b * textAlpha,
						255.00f * fade));
					brushText.setString(ui.element[i].textDisplay);
					floatRect = brushText.getGlobalBounds();
					brushText.setOrigin(0.00f, 0.00f);
					if (!ui.element[i].ignoreOrigin) { brushText.setOrigin(math.round(floatRect.width / 2.00f), math.round(floatRect.height / 2.00f) + 4); }
					brushText.setPosition(ui.element[i].pos + ui.element[i].textOffset);
					window.texHandleTop.draw(brushText, miniMatrix);
				}
				// Tooltip
				ui.element[i].tooltip.display();
			}
		}
	}
	// Targeting
	if (target.active && !target.activeBuild)
	{
		brushRect.setTexture(&visual.gameTex[database.texture[TEX_MOUSE_TARGET]].handle, true);
		brushRect.setSize(vec2f(40, 40));
		brushRect.setOrigin(vec2f(20, 20));
		brushRect.setPosition(window.getMousePos(false));
		brushRect.setFillColor(color(255, 255, 255));
		window.texHandleTop.draw(brushRect);
	}
	ui.access.unlock();
}

void cWindow::paintDebugInfo()
{
	if (!core.debugMode) { return; }
	sf::Transform miniMatrix;
	miniMatrix.scale(sf::Vector2f(settings.sampleMod, settings.sampleMod));

	brushRect.setOrigin(0, 0);
	brushRect.setPosition(0, 0);
	brushRect.setSize(sf::Vector2f(150, 70));
	brushRect.setTexture(0);
	brushRect.setFillColor(sf::Color(0, 0, 0, 150));
	// Unit orders
	if (core.advancedDebug)
	{
		float dist, angle;
		brushCircle.setFillColor(sf::Color(0, 255, 0));
		//brushCircle.setOutlineThickness(-1.00f);
		brushCircle.setOutlineColor(sf::Color(0, 0, 0));
		for (int i = 0; i < game.unitCounter; i++)
		{
			// First order
			if (game.unit[i].orderCounter > 0)
			{
				dist = math.getDistance(game.unit[i].pos, game.unit[i].order[0].targetPos);
				angle = math.getAngle(game.unit[i].pos, game.unit[i].order[0].targetPos);
				brushRect.setSize(vec2(dist, 2.00f));
				brushRect.setOrigin(0.00f, 1.00f);
				brushRect.setPosition(game.unit[i].pos);
				brushRect.setRotation(angle);
				brushRect.setFillColor(sf::Color(0, 255, 0));
				//brushRect.setOutlineThickness(-1.00f);
				brushRect.setOutlineColor(sf::Color(0, 0, 0));
				window.texHandleTop.draw(brushRect, window.matrixHandle);
			}
			// Order queue
			for (int y = 0; y < game.unit[i].orderCounter; y++)
			{
				if (game.unit[i].order[y].type == ORDER_MOVETO && (game.unit[i].order[y + 1].type == ORDER_MOVETO 
					|| y == game.unit[i].orderCounter - 1))
				{
					if (y < game.unit[i].orderCounter - 1)
					{
						dist = math.getDistance(game.unit[i].order[y].targetPos, game.unit[i].order[y + 1].targetPos);
						angle = math.getAngle(game.unit[i].order[y].targetPos, game.unit[i].order[y + 1].targetPos);
						brushRect.setSize(vec2(dist, 2.00f));
						brushRect.setPosition(game.unit[i].order[y].targetPos);
						brushRect.setRotation(angle);
						window.texHandleTop.draw(brushRect, window.matrixHandle);
					}
					brushCircle.setPosition(game.unit[i].order[y].targetPos);
					brushCircle.setRadius(5.00f);
					brushCircle.setOrigin(5.00f, 5.00f);
					window.texHandleTop.draw(brushCircle, window.matrixHandle);
				}
			}
			brushCircle.setPosition(game.unit[i].pos);
			brushCircle.setRadius(game.unit[i].collisionDistance);
			brushCircle.setOrigin(game.unit[i].collisionDistance, game.unit[i].collisionDistance);
			window.texHandleTop.draw(brushCircle, window.matrixHandle);
		}
		brushRect.setRotation(0.00f);
		brushRect.setOutlineThickness(0.00f);
	}

	// Camera rect
	if (core.advancedDebug)
	{
		brushRect.setPosition(camera.pos);
		brushRect.setFillColor(sf::Color(0, 0, 0, 0));
		brushRect.setOutlineColor(sf::Color(0, 255, 0, 255));
		brushRect.setSize(vec2f(camera.res.x, camera.res.y));
		brushRect.setOutlineThickness(-10.00f);
		window.texHandleTop.draw(brushRect, window.matrixHandle);
		brushRect.setScale(vec2f(1, 1));
		brushRect.setOutlineThickness(0);
	}

	// Debug text
	string text;
	brushText.setCharacterSize(16);
	brushText.setOrigin(0, 0);
	brushText.setColor(sf::Color(255, 255, 255, 255));
	brushText.setFont(visual.fontMain);

	// Overlay
	brushRect.setOrigin(0, 0);
	brushRect.setPosition(0, 0);
	brushRect.setSize(sf::Vector2f(150, 90));
	brushRect.setTexture(0);
	brushRect.setFillColor(sf::Color(0, 0, 0, 150));
	window.texHandleTop.draw(brushRect, miniMatrix);

	// FPS (Window thread)
	brushText.setPosition(5, 0);
	text = "FPS: " + to_string(core.thread_windowTicksPerSec);
	brushText.setString(text);
	window.texHandleTop.draw(brushText, miniMatrix);

	// Timers (Server world thread)
	brushText.setPosition(5, 20);
	text = "World: " + to_string(core.thread_serverWorldTicksPerSec);
	brushText.setString(text);
	window.texHandleTop.draw(brushText, miniMatrix);

	// Server ping
	brushText.setPosition(5, 40);
	text = "Ping: " + to_string(client.ping) + " ms";
	brushText.setString(text);
	window.texHandleTop.draw(brushText, miniMatrix);

	// Units
	brushText.setPosition(5, 70);
	text = "Units: " + to_string(visual.unitsPainted) + " / " + to_string(game.unitCounter);
	brushText.setString(text);
	window.texHandleTop.draw(brushText, miniMatrix);
}

void cWindow::paintConsole()
{
	if (!console.displayed) { return; }
	sf::Transform miniMatrix;
	miniMatrix.scale(sf::Vector2f(settings.sampleMod, settings.sampleMod));

	brushRect.setOrigin(0, 0);
	brushRect.setPosition(0, 0);
	brushRect.setSize(vec2f(camera.res.x, camera.res.y));
	brushRect.setTexture(0);
	brushRect.setFillColor(sf::Color(0, 0, 0, 235));
	if (core.serverMode) { brushRect.setFillColor(sf::Color(0, 0, 0, 255)); }
	window.texHandleTop.draw(brushRect, miniMatrix);

	brushText.setFont(visual.fontConsole);
	brushText.setCharacterSize(settings.consoleFontSize);
	brushText.setColor(sf::Color(0, 255, 0));
	brushText.setPosition(vec2f(5.00f, camera.res.y - settings.consoleFontSize - 5));
	brushText.setOrigin(vec2f(0.00f, 0.00f));
	brushText.setString("> " + console.inputDisplay + "_");
	window.texHandleTop.draw(brushText, miniMatrix);

	for (int i = max(0, (int)console.history[console.displayedPage].size() - console.getLineCount() - console.scrollOffset); i < (int)console.history[console.displayedPage].size() - max(0, console.scrollOffset); i++)
	{
		console.access.lock();
		brushText.setPosition(vec2f(5.00f, (i - max(0, (int)console.history[console.displayedPage].size() - console.getLineCount()) + console.scrollOffset) * 
			(settings.consoleFontSize + settings.consoleLineSpacing) ));
		brushText.setString(console.history[console.displayedPage][i]);
		window.texHandleTop.draw(brushText, miniMatrix);
		console.access.unlock();
	}
}

void cUITooltip::display()
{
	if (text.length() == 0 || alpha <= 0.00f) { return; }

	sf::FloatRect floatRect;
	sf::Transform miniMatrix;
	miniMatrix.scale(sf::Vector2f(settings.sampleMod, settings.sampleMod));

	// Initializing the text
	//brushText.setString(sf::String(text, locale("russian")));
	brushText.setString(textDisplay);
	brushText.setPosition(math.round(pos.x + offset.x), math.round(pos.y + offset.y));
	brushText.setOrigin(0.00f, 0.00f);
	brushText.setCharacterSize(16);
	if (!ignoreOrigin) {
		brushText.setOrigin(math.round(brushText.getGlobalBounds().width / 2), 0);
	}
	brushText.setColor(sf::Color(255, 255, 255, alpha));
	// Bound to window rect
	floatRect = brushText.getGlobalBounds();
	if (floatRect.left - 3 < 0) { brushText.move(-floatRect.left + 3, 0.00f); }
	else if (floatRect.left + floatRect.width + 3 > camera.res.x) { brushText.move(camera.res.x - floatRect.left - floatRect.width - 3, 0.00f); }
	if (floatRect.top - 3 < 0) { brushText.move(0.00f, -floatRect.top + 3); }
	// Drawing text backgrund
	floatRect = brushText.getGlobalBounds();
	brushRect.setPosition(floatRect.left - 3, floatRect.top - 3);
	brushRect.setOrigin(0, 0);
	brushRect.setSize(sf::Vector2f(floatRect.width + 6, floatRect.height + 6));
	brushRect.setFillColor(sf::Color(0, 0, 0, alpha));
	brushRect.setTexture(0);
	brushRect.setOutlineColor(sf::Color(190, 190, 190, alpha));
	brushRect.setOutlineThickness(-1.00f);
	//brushRect.setTexture(&visual.gameTex[database.texture[TEX_UI_TOOLTIP]].handle);
	/*brushRect.setTextureRect(sf::IntRect(0, 0, visual.gameTex[database.texture[TEX_UI_TOOLTIP]].handle.getSize().x,
		visual.gameTex[database.texture[TEX_UI_TOOLTIP]].handle.getSize().y));*/
	window.texHandleTop.draw(brushRect, miniMatrix);

	brushRect.setOutlineThickness(0.00f);
	// Drawing the text on top
	window.texHandleTop.draw(brushText, miniMatrix);
}