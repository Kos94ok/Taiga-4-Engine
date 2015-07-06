
#include "chat.h"
#include "camera.h"
#include "settings.h"
#include "ui.h"

void cChat::show(bool focus)
{
	inFocus = focus;
	displayed = true;
	ui.wndChat.open();
	if (!inFocus) { noFocusTimer = 2.00f; }
}

void cChat::hide()
{
	ui.wndChat.close();
	inFocus = false;
	displayed = false;
	clearInput();
}

void cChat::toggle()
{
	if (!displayed) { show(false); displayedPage = CHATTAB_ALL; }
	/*else if (displayedPage == SUBCMD_ALL && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) { displayedPage = SUBCMD_INFO; }
	else if (displayedPage == SUBCMD_INFO && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) { displayedPage = SUBCMD_ECHO; }
	else if (displayedPage == SUBCMD_ECHO && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) { displayedPage = SUBCMD_ERROR; }
	else if (displayedPage == SUBCMD_ERROR && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) { displayedPage = SUBCMD_DEBUG; }
	else if (displayedPage == SUBCMD_DEBUG && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) { displayedPage = SUBCMD_ALL; }*/
	else { hide(); }
	scrollOffset = 0;
}

int cChat::getLineCount()
{
	return floor((camera.res.y - (float)settings.consoleFontSize * 1.50) / (settings.consoleFontSize + settings.consoleLineSpacing));
}

void cChat::scroll(int direction)
{
	scrollOffset += direction * settings.consoleScrollSpeed;
	if (scrollOffset < 0) { scrollOffset = 0; }
	if (scrollOffset > max(0, (int)history[displayedPage].size() - getLineCount())) { scrollOffset = max(0, (int)history[displayedPage].size() - getLineCount()); }
}

void cChat::resetScroll()
{
	scrollOffset = 0;
}

void cChat::clear()
{
	history[console.displayedPage].clear();
	resetScroll();
	if (displayedPage == SUBCMD_ALL)
	{
		history[SUBCMD_ALL].push_back("[CONSOLE] All Messages Tab");
		history[SUBCMD_ALL].push_back("============================");
	}
	else if (displayedPage == SUBCMD_INFO)
	{
		history[SUBCMD_INFO].push_back("[CONSOLE] Info Messages Tab");
		history[SUBCMD_INFO].push_back("============================");
	}
	else if (displayedPage == SUBCMD_ECHO)
	{
		history[SUBCMD_ECHO].push_back("[CONSOLE] Echo Messages Tab");
		history[SUBCMD_ECHO].push_back("============================");
	}
	else if (displayedPage == SUBCMD_ERROR)
	{
		history[SUBCMD_ERROR].push_back("[CONSOLE] Error Messages Tab");
		history[SUBCMD_ERROR].push_back("============================");
	}
	else if (displayedPage == SUBCMD_DEBUG)
	{
		history[SUBCMD_DEBUG].push_back("[CONSOLE] Debug Messages Tab");
		history[SUBCMD_DEBUG].push_back("============================");
	}
}

cChat& operator << (cChat& target, std::string str)
{
	target.output(str);
	return target;
}

cChat& operator << (cChat& target, int i)
{
	target.output(to_string(i));
	return target;
}

cChat& operator << (cChat& target, float f)
{
	target.output(to_string(f));
	return target;
}

cChatTab& operator << (cChatTab& target, std::string str)
{
	chat.output(str, target.type);
	return target;
}

cChatTab& operator << (cChatTab& target, int i)
{
	chat.output(to_string(i), target.type);
	return target;
}

cChatTab& operator << (cChatTab& target, float f)
{
	chat.output(to_string(f), target.type);
	return target;
}