
#include "main.h"

string cSave::getChunkFilePath(vec2i pos)
{
	return "Savefiles//" + save.savefileName + "//" + save.worldName + "//" + to_string(pos.x) + "-" + to_string(pos.y) + ".chunk";
}