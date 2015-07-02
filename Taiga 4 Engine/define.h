
#define	LINESOFCODE_01_02_2015			6372
#define LINESOFCODE_31_05_2015			9050

#define LIMIT_CMD						64
#define LIMIT_TILEMAP					64
#define LIMIT_TEXTURE					256
#define LIMIT_ORDERS					128
#define LIMIT_ANIMATIONS				16
#define LIMIT_REFERENCE					64
#define LIMIT_SHADERS					32
#define LIMIT_DESCR						8
#define LIMIT_COMPONENTS				8
#define LIMIT_DISMANTLE					8
#define LIMIT_RECIPES					64
#define LIMIT_ITEMS						128
#define LIMIT_DROP						16
#define LIMIT_UNIT						2048
#define LIMIT_DB_UNIT					64
#define LIMIT_DB_ITEM					64
#define LIMIT_DB_UIELEMENTS				32
#define LIMIT_DB_TEXTURES				32
#define LIMIT_UI_ELEMENTS				128
#define LIMIT_UI_PRIORITY				16
#define LIMIT_SERVER_PLAYERS			32
#define LIMIT_SERVER_PACKETQUEUE		2048
#define LIMIT_MAP						64
#define LIMIT_BUTTONSPERCOLUMN			5
#define LIMIT_ACTIVEBUTTONS				35
#define LIMIT_SELECTION					64
#define LIMIT_CHUNKSIZE					500
#define LIMIT_ICONSIZE					24
#define LIMIT_SCRIPTCOUNT				64
#define LIMIT_SOUND						64

#define STATE_UNDEFINED					-1
#define STATE_MAINMENU					0
#define STATE_GAME						1
#define STATE_GAMEMENU					2

#define ORDER_IDLE						0
#define ORDER_MOVETO					1
#define ORDER_PICKUP					2
#define ORDER_HARVEST					3
#define ORDER_DEATH						4

#define ANIM_IDLE						0
#define ANIM_MOVE						1
#define ANIM_ATTACK						2
#define ANIM_WORK						3
#define ANIM_PICKUP						4
#define ANIM_DEATH						5

#define MSG_CMD							1
#define MSG_PING						2
#define MSG_PONG						3
#define MSG_ECHO						4
#define MSG_GAME_TIME					5
#define MSG_GAME_LIGHT					6
#define MSG_WORLD_CHUNKDATA				7
#define MSG_UI_UPDATEITEMLIST			8
#define MSG_CAMERA_MOVETO				9
#define MSG_UNIT_HERO					50
#define MSG_UNIT_ADD					51
#define MSG_UNIT_REMOVE					52
#define MSG_UNIT_SETHEALTH				53
#define MSG_UNIT_SETMAXHEALTH			54
#define MSG_UNIT_ADDITEM				55
#define MSG_UNIT_REMOVEITEM				56
#define MSG_UNIT_SETRESOURCE			57
#define MSG_UNIT_MOVETO					58
#define MSG_ORDER_MOVETO				100
#define MSG_ORDER_MOVETOPATH			101
#define MSG_ORDER_PICKUP				102
#define MSG_ORDER_HARVEST				103
#define MSG_ORDER_STOP					104
#define MSG_ORDER_DEATH					105
#define MSG_CONTROLS_MOVETO				150
#define MSG_CONTROLS_PICKUP				151
#define MSG_CONTROLS_HARVEST			152
#define MSG_CONTROLS_STOP				153
#define MSG_CONTROLS_CAMERA				154
#define MSG_CONTROLS_CRAFTITEM			160
#define MSG_CONTROLS_DROPITEM			161
#define MSG_CONTROLS_DISMANTLE			162
#define MSG_CONTROLS_BLINK				170
#define MSG_CONTROLS_RIFLE				171
#define MSG_CONTROLS_AXE				172
#define MSG_CONTROLS_PICKAXE			173
#define MSG_CONTROLS_CONSUME			174
#define MSG_REQUEST_CHUNKDATA			180
#define MSG_INFO_CAMRES					190
#define MSG_BIG_WORLD_ONECHUNK			500
#define MSG_BIG_WORLD_ALLCHUNKS			501

#define PLAYERS_ALL						-1
#define PLAYERS_REMOTE					-2

#define MOUSE_FREE						0
#define MOUSE_CONTROLS_MOVEMENT			1
#define MOUSE_CONTROLS_UNITCLICK		2
#define MOUSE_UI_BUTTON					3
#define MOUSE_BUSY						4

#define REF_UI_MAINMENU					0
#define REF_UI_GAME						1
#define REF_UI_ICON						2
#define REF_UI_BUTTON					3
#define REF_UI_IMAGE					4
#define REF_UI_TEXT						5
#define REF_UI_INVENTORY				6		// Object is related to inventory window
#define REF_UI_INVENTORY_FULL			7		// Object is related to full inventory window
#define REF_UI_INVENTORY_FULL_BG		8
#define REF_UI_INVENTORY_BUTTON			9		// Object is the button that opens inventory
#define REF_UI_INVENTORY_ITEM			10		// Object is the button that is linked to some item
#define REF_UI_INVENTORY_DESCR			11		// Object is part of the hovered item description
#define REF_UI_HEALTHBAR				12
#define REF_UI_INVENTORY_WEIGHT			13		// Object is a total weight description
#define REF_UI_CONTEXTMENU				14		// Object belongs to the context menu
#define REF_UI_ACTIVEITEM				15		// Object is one of the active equipped item buttons
#define REF_UNIT_PICKUP					0		// Unit can be picked up as an item
#define REF_UNIT_HARVESTABLE			1		// Unit can be harvested for the resource
#define REF_UNIT_NOSHADOW				2		// Unit can not create shadow
#define REF_UNIT_ALWAYSVISIBLE			3		// Unit will always be rendered on the screen
#define REF_UNIT_MISSILE				4		// Unit is a projectile
#define REF_UNIT_ROTATE					5		// Unit's sprite is rotated to the facing angle
#define REF_UNIT_NOSAVE					6		// Unit is not saved
#define REF_UNIT_NORENDER				7		// Unit is not displayed on the screen
#define REF_UNIT_NOUNLOAD				8		// Unit is never unloaded from memory
#define REF_UNIT_BESTPATHING			9		// Unit's position is validated before moving
#define REF_UNIT_TREE					10		// Unit can be harvested with axe
#define REF_UNIT_STONE					11		// Unit can be harvested with pickaxe
#define REF_UNIT_DAMAGE_RIFLE			12		// Unit deals the damage of the rifle on impact
#define REF_UNIT_NOSELECTION			13		// Unit is not selectable
#define REF_ITEM_EQUIP					0		// Item can be equipped
#define REF_ITEM_ACTIVE					1		// Item can be activated
#define REF_ITEM_CONSUME				2		// Item can be consumed
#define REF_ITEM_NODROP					3		// Item can not be dropped
#define REF_ITEM_NOCRAFT				4		// Item can not be used in crafting
#define REF_ITEM_BLINK					5		// Item can teleport the user to target point
#define REF_ITEM_RIFLE					6		// Item can shoot bullets
#define REF_ITEM_AXE					7		// Item can be used to chop wood
#define REF_ITEM_PICKAXE				8		// Item can be used to chop stone
#define REF_ITEM_EDITOR					9		// Item is editor-only
#define REF_ITEM_UNPACKABLE				10		// Item context menu will display 'unpack' instead of 'dismantle'
#define REF_ITEM_EMPTIABLE				11		// Item context menu will display 'empty' instead of 'dismantle'
#define REF_ITEM_DISMANTLE_ON_CONSUME	12		// Item is dismantled when it is consumed
#define REF_ITEM_GHOST					13		// Item is a non-interactive ghost item

#define TEX_UI_TOOLTIP					1
#define TEX_WORLD_GROUND				2
#define TEX_ICON_WINDOW					3
#define TEX_SELECTION_CIRCLE			4

#define SHADER_FULLSCREEN				1
#define SHADER_BLOOM					2
#define SHADER_LIGHT					3
#define SHADER_SHADOW					4
#define SHADER_CAMBLUR					5

#define INV_FULL						0
#define INV_SIDE						1
#define INV_CHAR						2

#define FONT_MAIN						0
#define FONT_DESCR						1

#define CONTEXTMENU_NOTHING				0
#define CONTEXTMENU_GROUND				1
#define CONTEXTMENU_UNIT				2
#define CONTEXTMENU_ITEM				3

#define CATEGORY_EQUIPPED				-1
#define CATEGORY_ALL					0
#define CATEGORY_WEAPONS				1
#define CATEGORY_ARMOR					2
#define CATEGORY_MATERIALS				3
#define CATEGORY_CONSUMABLES			4
#define CATEGORY_OTHER					5

#define MSG_NOSHADERSUPPORT				1
#define MSG_SHADERCOMPILE_UNIVERSAL		2
#define MSG_SHADERCOMPILE_LIGHT			3
#define MSG_SHADERCOMPILE_OTHER			4
#define MSG_TEXTURESIZE_LIMIT			5

#define VIDEO_INTEL						0
#define VIDEO_NVIDIA					1
#define VIDEO_ATI						2
#define VIDEO_MICROSOFT					3
#define VIDEO_OTHER						4

#define DIR_RIGHT						0
#define DIR_UP							1
#define DIR_LEFT						2
#define DIR_DOWN						3

#define CHUNK_UNDEFINED					0
#define CHUNK_NORMAL					1
#define CHUNK_BLOCKED					2
#define CHUNK_SPAWN						3
#define CHUNK_VILLAGE					4
#define CHUNK_LOOT_COMMON				5
#define CHUNK_LOOT_GOOD					6
#define CHUNK_LOOT_RARE					7
#define CHUNK_LOOT_PERFECT				8

#define SORT_BYNAME						0
#define SORT_BYTYPE						1

#define SUBCMD_ALL						0
#define SUBCMD_INFO						1
#define SUBCMD_ECHO						2
#define SUBCMD_ERROR					3
#define SUBCMD_DEBUG					4

#define POWER_HAND						0
#define POWER_STONE						1
#define POWER_FLINT						2
#define POWER_IRON						3
#define POWER_STEEL						4
#define POWER_IMBA						5