
#include "main.h"
#include "chat.h"
#include "UI.h"

void cChat::logMessage(int msg, cArg args)
{
	if (core.serverMode) { return; }
	int argi[4];
	string type;
	switch (msg)
	{
	case LOGMSG_ITEM_ADD:
		// Args: Item type, Item count
		type = args[0];		stringstream(args[1]) >> argi[1];
		log << "Received " << type << " x" << argi[1] << endl;
		break;
	case LOGMSG_ITEM_REMOVE:
		// Args: Item type, Item count
		type = args[0];		stringstream(args[1]) >> argi[1];
		log << "Lost " << type << " x" << argi[1] << endl;
		break;
	}
	ui.updateChatWindow();
}