
#include "chat.h"
#include "main.h"
#include "UI.h"

cChat::cChat()
{
	main.type = CHATTAB_ALL;
	players.type = CHATTAB_PLAYERS;
	log.type = CHATTAB_LOG;

	inFocus = false;
	displayed = false;
}

void cChat::output(std::string str, int chatTab)
{
	string backup;

	waitingQueue += str;
	if (str.length() >= 1 && str.substr(str.length() - 1) == "\n")
	{
		backup = waitingQueue;
		// Adding time string
		if (waitingQueue.length() > 1) {
			waitingQueue = util.getCurrentTimeString() + " - " + waitingQueue;
		}

		// Flushing to file
		ofstream file;
		if (!core.serverMode) { file.open("Logs//chat.txt", ios::app); }
		file << waitingQueue;
		file.close();

		// Returning to backup if no timestamp is needed
		if (chatTab == CHATTAB_LOG) {
			waitingQueue = backup;
		}

		// Flushing to console
		history[CHATTAB_ALL].push_back(sf::String(waitingQueue, locale("russian")));
		history[chatTab].push_back(sf::String(waitingQueue, locale("russian")));
		waitingQueue.clear();
		if (scrollOffset != 0 && (int)history[CHATTAB_ALL].size() > getLineCount())
		{
			scrollOffset += 1;
		}

		// Update UI
		//if (displayed) { ui.wndChat.update(); }
		//else { show(false); }
		ui.updateChatWindow();
	}
}

void cChat::addToInput(sf::String str)
{
	input.append(str.toAnsiString(locale("russian")));
	inputDisplay += str;
}

void cChat::removeLastFromInput()
{
	if (input.length() == 0) { return; }
	input.erase(input.length() - 1);
	inputDisplay.erase(input.length());
}

void cChat::flushInput()
{
	console.scrollOffset = 0;
	console.echo << "> " << input << endl;
	if (input.length() == 0) { return; }

	//parseCommand(input);

	clearInput();
}

void cChat::clearInput()
{
	input.clear();
	inputDisplay.clear();
}