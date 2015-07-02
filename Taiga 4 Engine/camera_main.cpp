
#include "main.h"
#include "camera.h"
#include "client.h"
#include "UI.h"
#include "window.h"

bool cCamera::isMoving()
{
	return (moveVector.x != 0.00f || moveVector.y != 0.00f);
}

void cCamera::move(sf::Vector2f offset)
{
	camera.pos += offset;

	sf::Packet data;
	// Camera position update
	if (client.connected && !core.localServer)
	{
		data << MSG_CONTROLS_CAMERA << pos.x << pos.y;
		client.sendPacket(data);
		data.clear();
	}
	// Mouse hold order
	if (client.connected && ui.mouseStateLMB == MOUSE_CONTROLS_MOVEMENT && moveUpdateTimer <= 0.00f)
	{
		moveUpdateTimer = 0.05f;
		sf::Vector2f mousePos = window.getMousePos(true);
		// Sending data to server
		data << MSG_CONTROLS_MOVETO << mousePos.x << mousePos.y << false;
		client.sendPacket(data);
		data.clear();
	}
}

void cCamera::moveto(sf::Vector2f newPos)
{
	move(newPos - pos - sf::Vector2f(res.x / 2.00f, res.y / 2.00f));
}

void cCamera::setZoom(float zoom)
{
	if (zoomFactor != zoom)
	{
		zoomFactor = zoom;
		// Enable the blur
		moveVector.x = 0.50f;
		moveVector.y = 0.50f;
	}
}

void cCamera::adjustZoom(int delta)
{
	setZoom(zoomFactor + zoomFactor * 0.20f * delta);
	if (!core.advancedDebug)
	{
		if (zoomFactor > 7.00f) { setZoom(7.00f); }
		if (zoomFactor < 1.00f) { zoomFactor = 1.00f; }
	}
}