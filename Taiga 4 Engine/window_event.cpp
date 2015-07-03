
#include "main.h"
#include "window.h"
#include "game.h"
#include "settings.h"
#include "UI.h"
#include "editor.h"
#include "visual.h"
#include "client.h"
#include "camera.h"
#include "target.h"

void cWindow::mainEvent()
{
	sf::Event eventPoll;
	sf::Packet data;
	sf::Vector2f mousePos;
	string buf;
	wstring wbuf;
	while (window.winHandle.pollEvent(eventPoll))
	{
		// Targeting
			// Use
		if (target.active && eventPoll.type == sf::Event::MouseButtonPressed && eventPoll.mouseButton.button == sf::Mouse::Left)
		{
			target.apply();
			ui.mouseStateLMB = MOUSE_BUSY;
		}
			// Reset
		if (target.active && eventPoll.type == sf::Event::MouseButtonPressed && eventPoll.mouseButton.button == sf::Mouse::Right)
		{
			target.reset();
			ui.mouseStateRMB = MOUSE_BUSY;
		}
		// Interface
			// Interface button click
		if (eventPoll.type == sf::Event::MouseButtonPressed && (eventPoll.mouseButton.button == sf::Mouse::Left
			|| eventPoll.mouseButton.button == sf::Mouse::Right))
		{
			bool toMarkLeft = false, toMarkRight = false;
			mousePos = window.getMousePos();
			for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
			{
				if (ui.element[i].isValid)
				{
					if (ui.element[i].button.action.length() > 0 && ui.element[i].isHovered
						&& (ui.contextMenuTarget == CONTEXTMENU_NOTHING || ui.element[i].hasRef(REF_UI_CONTEXTMENU))
						&& (!settings.enableQuickCast || !ui.element[i].hasRef(REF_UI_ACTIVEITEM)) )
					{
						if (eventPoll.mouseButton.button == sf::Mouse::Left && ui.mouseStateLMB == MOUSE_FREE)
						{
							if (ui.element[i].button.action == "-") { toMarkLeft = true; }
							else { ui.mouseStateLMB = MOUSE_UI_BUTTON; }

							if (ui.clickTimer <= 0.00f)
							{
								// Force lazy cast if the mouse is on the button
								if (ui.element[i].hasRef(REF_UI_ACTIVEITEM) && util.intersects(window.getMousePos(), ui.element[i].pos - ui.element[i].size / 2.00f, ui.element[i].size))
								{
									target.enable_forButton(ui.element[i].globalId);
								}
								// Don't allow hold and click if lazy cast is enabled
								else if (!ui.element[i].hasRef(REF_UI_ACTIVEITEM) || !settings.enableLazyCast)
								{
									ui.element[i].button.callbackLeft(ui.element[i].globalId);
								}
							}
						}
						else if (eventPoll.mouseButton.button == sf::Mouse::Right && ui.mouseStateRMB == MOUSE_FREE)
						{
							if (ui.element[i].button.action == "-") { toMarkRight = true; }
							else { ui.mouseStateRMB = MOUSE_UI_BUTTON; }

							if (ui.clickTimer <= 0.00f) { ui.element[i].button.callbackRight(ui.element[i].globalId); }
						}
					}
				}
			}
			if (toMarkLeft) { ui.mouseStateLMB = MOUSE_UI_BUTTON; ui.clickTimer = 0.08f; }
			if (toMarkRight) { ui.mouseStateRMB = MOUSE_UI_BUTTON; ui.clickTimer = 0.08f; }
		}
			// Close the context menu
		if (eventPoll.type == sf::Event::MouseButtonPressed
			&& (
				(eventPoll.mouseButton.button == sf::Mouse::Left && ui.mouseStateLMB != MOUSE_UI_BUTTON)
				|| (eventPoll.mouseButton.button == sf::Mouse::Right && ui.mouseStateRMB != MOUSE_UI_BUTTON)
			)
			&& ui.contextMenuTarget != CONTEXTMENU_NOTHING)
		{
			ui.clearContextMenu();
			if (eventPoll.mouseButton.button == sf::Mouse::Left) { ui.mouseStateLMB = MOUSE_UI_BUTTON; }
			else { ui.mouseStateRMB = MOUSE_UI_BUTTON; }
		}
		// =========================================================
		// =========================================================
		// Hotkeys
		if (eventPoll.type == sf::Event::KeyReleased)
		{
			// Console
			if (eventPoll.key.code == settings.hkConsole) { console.toggle(); }
			else if (console.displayed)
			{
				if (eventPoll.key.code == sf::Keyboard::Up) { console.scrollHistory(-1); }
				else if (eventPoll.key.code == sf::Keyboard::Down) { console.scrollHistory(1); }
			}
			// Global hotkeys
			if (!console.displayed)
			{
				if (eventPoll.key.code == settings.hkDebugMode) { core.debugMode = !core.debugMode; }
				if (eventPoll.key.code == settings.hkDebugAdvanced) {
					core.advancedDebug = !core.advancedDebug;
					if (core.advancedDebug) { core.debugMode = true; }
				}
				// Escape
				if (eventPoll.key.code == sf::Keyboard::Escape && ui.contextMenuTarget != CONTEXTMENU_NOTHING) { ui.clearContextMenu(); }
				else if (eventPoll.key.code == sf::Keyboard::Escape && ui.invOpened) { ui.closeInventory(); }
				// UI Element hotkeys
				for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
				{
					// Regular buttons
					if (ui.element[i].hasRef(REF_UI_INVENTORY_BUTTON) && eventPoll.key.code == settings.hkInventory)
					{
						// Quick cast or normal hotkey
						if (settings.enableQuickCast || !ui.element[i].hasRef(REF_UI_ACTIVEITEM))
						{
							ui.element[i].button.callbackLeft(i);
							i = LIMIT_UI_ELEMENTS;
						}
					}
					// Active items
					for (int y = 1; y < LIMIT_ACTIVEBUTTONS; y++)
					{
						if (ui.element[i].hasRef(REF_UI_ACTIVEITEM + y) && eventPoll.key.code == settings.hkActiveItem[y])
						{
							if (settings.enableQuickCast) {
								ui.element[i].button.callbackLeft(i);
							}
							else if (settings.enableLazyCast) {
								target.enable_forButton(ui.element[i].globalId);
							}
							i = LIMIT_UI_ELEMENTS;
						}
					}
				}
			}
		}
		// Text input
		else if (eventPoll.type == sf::Event::TextEntered)
		{
			if (console.displayed)
			{
				// Enter
				if (eventPoll.text.unicode == 13) { console.flushInput(); }
				// Backspace
				else if (eventPoll.text.unicode == 8) { console.removeLastFromInput(); }
				// Other
				else {
					sf::String convert(eventPoll.text.unicode);
					console.addToInput(convert);
				}
			}
		}
		// =========================================================
		// =========================================================
		// Unit control
			// Unit click
		if (eventPoll.type == sf::Event::MouseButtonPressed
			&& (
				(eventPoll.mouseButton.button == sf::Mouse::Left && ui.mouseStateLMB == MOUSE_FREE)
				|| (eventPoll.mouseButton.button == sf::Mouse::Right && ui.mouseStateRMB == MOUSE_FREE)
			))
		{
			mousePos = window.getMousePos(true);
			bool matchFound = false;
			int search;
			// Editor mode
			if (core.editorMode)
			{
				if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { editor.sel.clear(); }
				search = game.getUnitId(mousePos);
				if (search != -1)
				{
					editor.sel.add(search);
				}
			}
			// Regular mode
			else
			{
				//search = game.getUnitId(mousePos, REF_UNIT_PICKUP);
				search = visual.hoveredUnit;
				cUnit* target = &game.getUnit(search);
				// Item on the ground
				if (search != -1 && target->hasRef(REF_UNIT_PICKUP))
				{
					if (eventPoll.mouseButton.button == sf::Mouse::Left) {
						ui.mouseStateLMB = MOUSE_CONTROLS_UNITCLICK;
						data << MSG_CONTROLS_PICKUP << search << sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
						client.sendPacket(data);
						data.clear();
					}
					else {
						ui.mouseStateRMB = MOUSE_UI_BUTTON;
						ui.createContextMenuForUnit(search);
					}
				}
				// Harvestable object
				else if (search != -1 && target->hasRef(REF_UNIT_HARVESTABLE))
				{
					if (eventPoll.mouseButton.button == sf::Mouse::Left) {
						ui.mouseStateLMB = MOUSE_CONTROLS_UNITCLICK;
						data << MSG_CONTROLS_HARVEST << search << sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
						client.sendPacket(data);
						data.clear();
					}
					else {
						ui.mouseStateRMB = MOUSE_UI_BUTTON;
						ui.createContextMenuForUnit(search);
					}
				}
				// Ground click
				else
				{
					if (eventPoll.mouseButton.button == sf::Mouse::Left)
					{
						ui.mouseStateLMB = MOUSE_CONTROLS_MOVEMENT;
						// Sending data to server
						data << MSG_CONTROLS_MOVETO << mousePos.x << mousePos.y << sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
						client.sendPacket(data);
						data.clear();
					}
					else
					{
						ui.mouseStateRMB = MOUSE_UI_BUTTON;
						ui.createContextMenuForGround();
					}
				}
			}
		}
		// Mouse dragging (move order)
		if (eventPoll.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Left)
			&& !sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && ui.mouseStateLMB == MOUSE_CONTROLS_MOVEMENT)
		{
			mousePos = window.getMousePos(true);
			// Sending data to server
			if (client.connected) {
				data << MSG_CONTROLS_MOVETO << mousePos.x << mousePos.y << false;
				client.sendPacket(data);
			}
		}
		// =========================================================
		// =========================================================
		// Camera
			// Snap camera to character
		if (eventPoll.type == sf::Event::KeyPressed && eventPoll.key.code == settings.hkCamToHero && !console.displayed)
		{
			camera.moveto(game.unit[game.getUnitId(client.unit)].pos);
			camera.moveVector.x = 0.50f;
			camera.moveVector.y = 0.50f;
		}
			// Camera zoom
		if (eventPoll.type == sf::Event::MouseWheelMoved && !console.displayed)
		{
			camera.adjustZoom(eventPoll.mouseWheel.delta);
		}
			// Console scroll
		else if (eventPoll.type == sf::Event::MouseWheelMoved && console.displayed)
		{
			console.scroll(eventPoll.mouseWheel.delta);
		}
			// Reset camera zoom
		if (eventPoll.type == sf::Event::MouseButtonPressed && eventPoll.mouseButton.button == sf::Mouse::Middle && !console.displayed)
		{
			camera.setZoom(1.00f);
		}
			// Reset console position
		else if (eventPoll.type == sf::Event::MouseButtonPressed && eventPoll.mouseButton.button == sf::Mouse::Middle && console.displayed)
		{
			console.resetScroll();
		}
		// =========================================================
		// =========================================================
		// Random
			// Database reload
		if (core.debugMode && eventPoll.type == sf::Event::KeyPressed && eventPoll.key.code == sf::Keyboard::R && eventPoll.key.alt && eventPoll.key.control)
		{
			cmd_database_reload(NULL);
		}
			// Any mouse action
		if (eventPoll.type == sf::Event::MouseMoved)
		{
			if (sf::Vector2i(eventPoll.mouseMove.x, eventPoll.mouseMove.y) != ui.mouseLastPos) { ui.mouseNotMoving = 0.00f; }
			ui.mouseLastPos = sf::Vector2i(eventPoll.mouseMove.x, eventPoll.mouseMove.y);
		}
		if (eventPoll.type == sf::Event::MouseButtonPressed || eventPoll.type == sf::Event::KeyPressed)
		{
			ui.mouseNotMoving = 0.00f;
		}
			// Mouse release
		if (eventPoll.type == sf::Event::MouseButtonReleased)
		{
			if (eventPoll.mouseButton.button == sf::Mouse::Left) { ui.mouseStateLMB = MOUSE_FREE; }
			if (eventPoll.mouseButton.button == sf::Mouse::Right) { ui.mouseStateRMB = MOUSE_FREE; }
		}
			// Any mouse click
		if (eventPoll.type == sf::Event::MouseButtonPressed)
		{
			SetForegroundWindow(window.winHandle.getSystemHandle());
		}
			// Shutdown
		if (eventPoll.type == sf::Event::KeyPressed && eventPoll.key.code == sf::Keyboard::F4 && eventPoll.key.alt)
		{
			core.shutdown = true;
		}
			// Window closed
		if (eventPoll.type == sf::Event::Closed) { core.shutdown = true; }
	}
}