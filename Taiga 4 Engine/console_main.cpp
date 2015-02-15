
#include "main.h"

// Initializing the console class
cConsole::cConsole()
{
	for (int i = 0; i < LIMIT_CMD; i++) { console.cmdServerOnly[i] = true; }

	online = true;
	// Help command
	int cmdID = 0;
	console.cmdRegex[cmdID].assign("^help");
	console.cmdWrong[cmdID] = "help";
	console.cmdSyntax[cmdID] = "\"help\" - Display available commands";
	console.cmdFunc[cmdID] = cmd_help;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^macro [a-zA-Z0-9]+");
	console.cmdWrong[cmdID] = "macro";
	console.cmdSyntax[cmdID] = "\"macro [str]\" - Load the [macro] script";
	console.cmdFunc[cmdID] = cmd_macro;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.add [a-zA-Z0-9]+");
	console.cmdWrong[cmdID] = "unit.add";
	console.cmdSyntax[cmdID] = "\"unit.add [str] (num) (num)\" - Create a unit from [type] at (X) (Y)";
	console.cmdFunc[cmdID] = cmd_unit_add;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.moveto [0-9]+ [0-9]+ [0-9]+");
	console.cmdWrong[cmdID] = "unit.moveto";
	console.cmdSyntax[cmdID] = "\"unit.moveto [num] [num] [num] (bool)\" - Move a unit with [global ID] to [X] [Y]";
	console.cmdFunc[cmdID] = cmd_unit_moveto;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.setspeed [0-9]+ [0-9]+");
	console.cmdWrong[cmdID] = "unit.setspeed";
	console.cmdSyntax[cmdID] = "\"unit.setspeed [num] [num]\" - Set the [unit]'s movement speed to [value]";
	console.cmdFunc[cmdID] = cmd_unit_setspeed;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.remove [0-9]+");
	console.cmdWrong[cmdID] = "unit.remove";
	console.cmdSyntax[cmdID] = "\"unit.remove [num]\" - Remove a unit with [global ID]";
	console.cmdFunc[cmdID] = cmd_unit_remove;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.getlist");
	console.cmdWrong[cmdID] = "unit.getlist";
	console.cmdSyntax[cmdID] = "\"unit.getlist\" - Get the full list of units in memory";
	console.cmdFunc[cmdID] = cmd_unit_getlist;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.order.moveto [0-9]+ [0-9]+ [0-9]+");
	console.cmdWrong[cmdID] = "unit.order.moveto";
	console.cmdSyntax[cmdID] = "\"unit.order.moveto [num] [num] [num]\" - Order [unit] to go to [X] [Y]";
	console.cmdFunc[cmdID] = cmd_unit_order_moveto;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.order.getlist [0-9]+");
	console.cmdWrong[cmdID] = "unit.order.getlist";
	console.cmdSyntax[cmdID] = "\"unit.order.getlist [num]\" - Get the list of orders for [unit]";
	console.cmdFunc[cmdID] = cmd_unit_order_getlist;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.item.add [0-9]+ [a-zA-Z0-9]+");
	console.cmdWrong[cmdID] = "unit.item.add";
	console.cmdSyntax[cmdID] = "\"unit.item.add [num] [str] (amount)\" - Give [unit] one [item] (amount) times";
	console.cmdFunc[cmdID] = cmd_unit_item_add;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.item.remove [0-9]+ [a-zA-Z0-9]+");
	console.cmdWrong[cmdID] = "unit.item.remove";
	console.cmdSyntax[cmdID] = "\"unit.item.remove [num] [str] (amount)\" - Remove from [unit] one [item] (amount) times";
	console.cmdFunc[cmdID] = cmd_unit_item_remove;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^unit.item.getlist [0-9]+");
	console.cmdWrong[cmdID] = "unit.item.getlist";
	console.cmdSyntax[cmdID] = "\"unit.item.getlist [num]\" - Get the list of items for [unit]";
	console.cmdFunc[cmdID] = cmd_unit_item_getlist;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^camera.attach");
	console.cmdWrong[cmdID] = "camera.attach";
	console.cmdSyntax[cmdID] = "\"camera.attach\" - Attach the camera to controlled unit";
	console.cmdFunc[cmdID] = cmd_camera_attach;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^camera.lock");
	console.cmdWrong[cmdID] = "camera.lock";
	console.cmdSyntax[cmdID] = "\"camera.lock\" - Lock the camera to controlled unit";
	console.cmdFunc[cmdID] = cmd_camera_lock;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^game.timemod [0-9]+");
	console.cmdWrong[cmdID] = "game.timemod";
	console.cmdSyntax[cmdID] = "\"game.timemod [num]\" - Set the game speed to [percentage]";
	console.cmdFunc[cmdID] = cmd_game_timemod;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^game.settime [0-9.,]+");
	console.cmdWrong[cmdID] = "game.settime";
	console.cmdSyntax[cmdID] = "\"game.settime [num]\" - Set the game [time] of the day";
	console.cmdFunc[cmdID] = cmd_game_settime;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^game.setlight [0-9.,]+");
	console.cmdWrong[cmdID] = "game.setlight";
	console.cmdSyntax[cmdID] = "\"game.setlight [num]\" - Set the ambient light to [value]";
	console.cmdFunc[cmdID] = cmd_game_setlight;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^client.connect [0-9.:a-zA-Z]+");
	console.cmdWrong[cmdID] = "client.connect";
	console.cmdSyntax[cmdID] = "\"client.connect [ip]\" - Connect to the server at [address]";
	console.cmdFunc[cmdID] = cmd_client_connect;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^client.disconnect");
	console.cmdWrong[cmdID] = "client.disconnect";
	console.cmdSyntax[cmdID] = "\"client.disconnect\" - Disconnect from server";
	console.cmdFunc[cmdID] = cmd_client_disconnect;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^server.open");
	console.cmdWrong[cmdID] = "server.open";
	console.cmdSyntax[cmdID] = "\"server.open\" - Open the local server";
	console.cmdFunc[cmdID] = cmd_server_open;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^player.kick [0-9]+");
	console.cmdWrong[cmdID] = "player.kick";
	console.cmdSyntax[cmdID] = "\"player.kick [num]\" - Kick the [player] from server";
	console.cmdFunc[cmdID] = cmd_player_kick;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^player.setunit [0-9]+ [0-9]+");
	console.cmdWrong[cmdID] = "player.setunit";
	console.cmdSyntax[cmdID] = "\"player.setunit [num] [num]\" - Assign the [player]'s unit to [global id]";
	console.cmdFunc[cmdID] = cmd_player_setunit;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^player.getlist");
	console.cmdWrong[cmdID] = "player.getlist";
	console.cmdSyntax[cmdID] = "\"player.getlist\" - Display the list of players on local server";
	console.cmdFunc[cmdID] = cmd_player_getlist;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^ui.getlist");
	console.cmdWrong[cmdID] = "ui.getlist";
	console.cmdSyntax[cmdID] = "\"ui.getlist\" - Display the list of ui elements";
	console.cmdFunc[cmdID] = cmd_ui_getlist;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^editor.enable");
	console.cmdWrong[cmdID] = "editor.enable";
	console.cmdSyntax[cmdID] = "\"editor.enable\" - Enable the editor mode";
	console.cmdFunc[cmdID] = cmd_editor_enable;
	console.cmdServerOnly[cmdID] = false;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^editor.save [a-zA-Z0-9.]+");
	console.cmdWrong[cmdID] = "editor.save";
	console.cmdSyntax[cmdID] = "\"editor.save [str]\" - Save active blueprint as [file]";
	console.cmdFunc[cmdID] = cmd_editor_save;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^editor.load [a-zA-Z0-9.]+");
	console.cmdWrong[cmdID] = "editor.load";
	console.cmdSyntax[cmdID] = "\"editor.load [str]\" - Load [file] as active blueprint";
	console.cmdFunc[cmdID] = cmd_editor_load;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^editor.autogen");
	console.cmdWrong[cmdID] = "editor.autogen";
	console.cmdSyntax[cmdID] = "\"editor.autogen (num)\" - Generate (amount) of blueprints";
	console.cmdFunc[cmdID] = cmd_editor_autogen;
	cmdID += 1;
	console.cmdRegex[cmdID].assign("^exit");
	console.cmdWrong[cmdID] = "exit";
	console.cmdSyntax[cmdID] = "\"exit\" - Close the game";
	console.cmdFunc[cmdID] = cmd_exit;
	console.cmdServerOnly[cmdID] = false;
}

// Parsing user commands (mostly for debugging)
bool cConsole::parseCommand(string cmd)
{
	bool commandParsed = false;

	regex arg_any(" [a-zA-Z0-9._]+");						// Regex for any command argument
	int argCounter = 0;										// Amount of found arguments
	string args[] = { "0", "0", "0", "0" };					// Found arguments
	sregex_iterator it(cmd.begin(), cmd.end(), arg_any);	// Regex iterator for cmd
	sregex_iterator it_end;									// Iterator end condition

	// Getting arguments
	while (it != it_end && argCounter < 4)
	{
		args[argCounter++] = it->str().substr(1);	// Removing the first character (whitespace)
		++it;
	}

	// Parsing commands
	for (int i = 0; i < LIMIT_CMD; i++)
	{
		// If the command is syntactically correct, call the function
		if (regex_search(cmd, console.cmdRegex[i]) && console.cmdWrong[i].length() > 0)
		{
			commandParsed = true;
			if (!console.cmdServerOnly[i] || !client.connected || core.localServer) {
				console.cmdFunc[i](args);
			}
			else {
				cout << "[CMD] Only server can use this command!" << endl;
			}
		}
	}

	// Incorrect commands
	if (!commandParsed)
	{
		for (int i = 0; i < LIMIT_CMD; i++)
		{
			// If the command itself is correct, display correct argument syntax
			if (cmd.substr(0, console.cmdWrong[i].length()) == console.cmdWrong[i] && console.cmdWrong[i].length() > 0)
			{
				commandParsed = true;
				cout << "[CMD] " << console.cmdSyntax[i] << endl;
			}
		}
	}
	
	// Unknown command
	if (!commandParsed)
	{
		cout << "[CMD] Can not parse the command!" << endl;
	}

	return true;
}

// Console thread waits for another command from user
string cConsole::waitForCommand()
{
	string cmd = "";
	getline(cin, cmd);
	return cmd;
}

// Console main function
void consoleMain()
{
	cout << "[CMD] Console thread started" << endl;
	cout << "[CMD] Waiting for commands" << endl;
	while (!core.shutdown)
	{
		Sleep(100);
		console.parseCommand(console.waitForCommand());
	}
	cout << "[CMD] Cleaning up" << endl;
}