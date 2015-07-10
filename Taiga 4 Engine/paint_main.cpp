
#include "main.h"
#include "window.h"
#include "visual.h"
#include "settings.h"
#include "camera.h"
#include "database.h"
#include "saveload.h"
#include "game.h"
#include "math.h"

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
	// V-sync trap
	brushRect.setPosition(vec2f(-5000.00f, -5000.00f));
	window.winHandle.draw(brushRect);

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
		window.paintTileMap();
		window.paintUnits();
		window.paintLighting();
		window.paintPostFX();
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

	// Capturing screenshot
	if (util.screenshotRequested) {
		save.saveScreenshot(window.texHandleTop.getTexture().copyToImage());
	}

	window.winHandle.draw(buffer);
	window.winHandle.setVerticalSyncEnabled(settings.enableVertSync);
	window.winHandle.display();
	window.winHandle.setVerticalSyncEnabled(false);
	core.thread_windowTicks += 1;
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
		shader->setParameter("iSampleCount", 3.00f);
		shader->setParameter("sampleOffset", 0.0005f);
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