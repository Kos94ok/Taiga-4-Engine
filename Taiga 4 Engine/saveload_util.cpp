
#include "main.h"

string cSave::getChunkFilePath(vec2i pos)
{
	return "Savefiles//" + save.worldName + "//" + to_string(pos.x) + "-" + to_string(pos.y) + ".chunk";
}