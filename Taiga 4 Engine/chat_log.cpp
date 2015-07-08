
#include "main.h"
#include "chat.h"
#include "UI.h"
#include "database.h"

void cChat::logMessage(int msg, cArg args)
{
	if (core.serverMode) { return; }
	int argi[4];
	string type, name;
	switch (msg)
	{
	case LOGMSG_ITEM_ADD:
		// Args: Item type, Item count
		type = args[0];		stringstream(args[1]) >> argi[1];
		log << "Received " << database.getItem(type).displayName << " x" << argi[1] << endl;
		break;
	case LOGMSG_ITEM_REMOVE:
		// Args: Item type, Item count
		type = args[0];		stringstream(args[1]) >> argi[1];
		log << "Lost " << database.getItem(type).displayName << " x" << argi[1] << endl;
		break;
	case LOGMSG_SCREENSHOT:
		// Args: Filename
		name = args[0];
		log << "Screenshot saved as " << name << endl;
	}
	ui.updateChatWindow();
	chat.noFocusTimer = value.chatNoFocusTimer;
}