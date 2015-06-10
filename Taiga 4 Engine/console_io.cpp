
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

void cConsole::output(std::string str)
{
	access.lock();

	waitingQueue += str;
	if (str.length() >= 1 && str.substr(str.length() - 1) == "\n")
	{
		outputQueue.push_back(waitingQueue);
		waitingQueue.clear();
	}

	access.unlock();
}