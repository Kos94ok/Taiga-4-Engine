
#include "precomp.h"
#include "util.h"

class cChatTab
{
public:
	int type;
};

class cChat
{
public:
	bool inFocus;
	bool displayed;
	int displayedPage;
	int scrollOffset;

	float noFocusTimer;

	cChatTab main;
	cChatTab players;
	cChatTab log;
	std::string input;
	sf::String inputDisplay;
	std::string waitingQueue;
	std::vector<sf::String> history[3];

	cChat();
	void show(bool focus);
	void hide();
	void toggle();
	void scroll(int dir);
	void resetScroll();
	void addToInput(sf::String str);
	void removeLastFromInput();
	void flushInput();
	int getLineCount();
	sf::Vector2f getPos();
	sf::Vector2f getSize();
	void clearInput();
	void clear();

	void logMessage(int message, cArg args);

	void output(std::string str, int chatTab = CHATTAB_LOG);
};

extern cChat chat;

cChat& operator << (cChat& target, std::string str);
cChat& operator << (cChat& target, int i);
cChat& operator << (cChat& target, float f);
cChatTab& operator << (cChatTab& target, std::string str);
cChatTab& operator << (cChatTab& target, int i);
cChatTab& operator << (cChatTab& target, float f);