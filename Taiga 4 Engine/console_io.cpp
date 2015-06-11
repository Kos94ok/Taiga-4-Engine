
#include "main.h"

cConsole& operator << (cConsole& cmd, std::string str)
{
	cmd.output(str);
	return cmd;
}

cConsole& operator << (cConsole& cmd, int i)
{
	cmd.output(to_string(i));
	return cmd;
}

cSubConsole& operator << (cSubConsole& cmd, std::string str)
{
	console.output(str, cmd.type);
	return cmd;
}

cSubConsole& operator << (cSubConsole& cmd, int i)
{
	console.output(to_string(i), cmd.type);
	return cmd;
}

void cConsole::output(std::string str, int subConsole)
{
	access.lock();

	waitingQueue += str;
	if (str.length() >= 1 && str.substr(str.length() - 1) == "\n")
	{
		outputQueue.push_back(waitingQueue);
		history[SUBCMD_ALL].push_back(sf::String(waitingQueue, locale("russian")));
		history[subConsole].push_back(sf::String(waitingQueue, locale("russian")));
		waitingQueue.clear();
		if (scrollOffset != 0 && (int)history[SUBCMD_ALL].size() > getLineCount())
		{
			scrollOffset += 1;
		}
	}

	access.unlock();
}

void cConsole::addToInput(sf::String str)
{
	input.append(str.toAnsiString(locale("russian")));
	inputDisplay += str;
}

void cConsole::removeLastFromInput()
{
	if (input.length() == 0) { return; }
	input.erase(input.length() - 1);
	inputDisplay.erase(input.length());
}

void cConsole::flushInput()
{
	console.echo << "> " << input << endl;
	parseCommand(input);

	clearInput();
}

void cConsole::clearInput()
{
	input.clear();
	inputDisplay.clear();
}