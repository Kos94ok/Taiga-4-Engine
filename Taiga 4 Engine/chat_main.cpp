
#include "chat.h"
#include "main.h"

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

void cChat::output(std::string str, int subConsole)
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
		/*if (!settings.enableConsoleTimestamps) {
			waitingQueue = backup;
		}*/

		// Flushing to console
		history[SUBCMD_ALL].push_back(sf::String(waitingQueue, locale("russian")));
		history[subConsole].push_back(sf::String(waitingQueue, locale("russian")));
		waitingQueue.clear();
		if (scrollOffset != 0 && (int)history[SUBCMD_ALL].size() > getLineCount())
		{
			scrollOffset += 1;
		}
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

cChat::cChat()
{
	inFocus = false;
	displayed = false;
}