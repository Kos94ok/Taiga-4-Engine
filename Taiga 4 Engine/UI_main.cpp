
#include "main.h"

int cUI::addElement(string type, sf::Vector2f pos)
{
	for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
	{
		if (!element[i].isValid)
		{
			element[i] = database.getUIElement(type);
			element[i].pos = pos;
			element[i].globalId = elementGlobalCounter++;
			element[i].isValid = true;
			lastCreatedElement = i;
			return elementGlobalCounter - 1;
		}
	}
	return -1;
}

int cUI::addElement(cUIElement elem, sf::Vector2f pos)
{
	for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
	{
		if (!element[i].isValid)
		{
			element[i] = elem;
			element[i].pos = pos;
			element[i].globalId = elementGlobalCounter++;
			element[i].isValid = true;
			lastCreatedElement = i;
			return elementGlobalCounter - 1;
		}
	}
	return -1;
}

void cUI::removeElement(int id)
{
	id = getElementId(id);
	if (id != -1)
	{
		element[id].isValid = false;
	}
}

void cUI::removeElementsByRef(int ref)
{
	for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
	{
		if (element[i].hasRef(ref))
		{
			//removeElement(element[i].globalId);
			element[i].isValid = false;
		}
	}
}

int cUI::getElementId(int id)
{
	for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
	{
		if (element[i].isValid && element[i].globalId == id) { return i; }
	}
	return -1;
}

bool cUIElement::hasText() { return (text.length() > 0); }
bool cUITooltip::hasText() { return (text.length() > 0); }

void cUIElement::setText(string txt)
{
	text = txt;
	textDisplay = sf::String(text, locale("russian"));
}

void cUITooltip::setText(string txt)
{
	text = txt;
	textDisplay = sf::String(text, locale("russian"));
}

int cUI::findByRef(int ref)
{
	for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
	{
		if (element[i].isValid && element[i].hasRef(ref)) { return i; }
	}
	return -1;
}

