
#include "precomp.h"

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

	cChatTab main;
	cChatTab players;
	cChatTab log;
	std::string input;
	sf::String inputDisplay;
	std::string waitingQueue;
	std::vector<sf::String> history[3];

	cChat();
	void show();
	void hide();
	void toggle();
	void scroll(int dir);
	void resetScroll();
	void addToInput(sf::String str);
	void removeLastFromInput();
	void flushInput();
	int getLineCount();
	void clearInput();
	void clear();

	void output(std::string str, int chatTab = CHATTAB_MAIN);
};

extern cChat chat;

cChat& operator << (cChat& target, std::string str);
cChat& operator << (cChat& target, int i);
cChat& operator << (cChat& target, float f);
cChatTab& operator << (cChatTab& target, std::string str);
cChatTab& operator << (cChatTab& target, int i);
cChatTab& operator << (cChatTab& target, float f);