
#include "main.h"

void cConsole::show()
{
	displayed = true;
#ifdef SYSTEM_WINDOWS
	ShowWindow(GetConsoleWindow(), SW_SHOW);
#endif
}

void cConsole::hide()
{
	displayed = false;
#ifdef SYSTEM_WINDOWS
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif
}

void cConsole::toggle()
{
	if (displayed) { hide(); }
	else { show(); }
}