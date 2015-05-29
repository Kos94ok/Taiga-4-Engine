
#include "define.h"

class cConsole
{
public:
	bool online;
	bool displayed;

	bool parseCommand(std::string cmd);
	std::string waitForCommand();

	bool cmdServerOnly[LIMIT_CMD];
	std::regex cmdRegex[LIMIT_CMD];
	std::string cmdWrong[LIMIT_CMD];
	std::string cmdSyntax[LIMIT_CMD];
	std::function<void(std::string[])> cmdFunc[LIMIT_CMD];

	cConsole();
	void show();
	void hide();
	void toggle();
};

void consoleMain();

void cmd_help(std::string args[]);
void cmd_macro(std::string args[]);
void cmd_unit_add(std::string args[]);
void cmd_unit_moveto(std::string args[]);
void cmd_unit_setspeed(std::string args[]);
void cmd_unit_remove(std::string args[]);
void cmd_unit_getlist(std::string args[]);
void cmd_unit_order_moveto(std::string args[]);
void cmd_unit_order_getlist(std::string args[]);
void cmd_unit_item_add(std::string args[]);
void cmd_unit_item_remove(std::string args[]);
void cmd_unit_item_getlist(std::string args[]);
void cmd_camera_attach(std::string args[]);
void cmd_camera_lock(std::string args[]);
void cmd_game_timemod(std::string args[]);
void cmd_game_settime(std::string args[]);
void cmd_game_setlight(std::string args[]);
void cmd_client_connect(std::string args[]);
void cmd_client_disconnect(std::string args[]);
void cmd_server_open(std::string args[]);
void cmd_player_kick(std::string args[]);
void cmd_player_setunit(std::string args[]);
void cmd_player_getlist(std::string args[]);
void cmd_ui_getlist(std::string args[]);
void cmd_editor_enable(std::string args[]);
void cmd_editor_save(std::string args[]);
void cmd_editor_load(std::string args[]);
void cmd_editor_autogen(std::string args[]);
void cmd_editor_setgentype(std::string args[]);
void cmd_database_reload(std::string args[]);
void cmd_exit(std::string args[]);