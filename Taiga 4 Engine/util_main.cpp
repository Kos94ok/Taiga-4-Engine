
#include "main.h"

void cUtil::detectVideoCard()
{
	string vendor = reinterpret_cast< char const * >(glGetString(GL_VENDOR));
	if (vendor == "Intel") {
		core.videoCard = VIDEO_INTEL;
		console << "[MAIN] Detected Intel video card" << "\n";
	}
	else if (vendor.substr(0, 6) == "NVIDIA") {
		core.videoCard = VIDEO_NVIDIA;
		console << "[MAIN] Detected Nvidia video card" << "\n";
	}
	else if (vendor.substr(0, 3) == "ATI") { 
		core.videoCard = VIDEO_ATI;
		console << "[MAIN] Detected AMD video card" << "\n";
	}
	else if (vendor.substr(0, 9) == "Microsoft") {
		core.videoCard = VIDEO_MICROSOFT;
		console << "[MAIN] Detected Microsoft video card" << "\n";
	}
	else {
		core.videoCard = VIDEO_OTHER;
		console << "[MAIN] Unknown video card: " << vendor << "\n";
	}
}

int cUtil::getInventorySortingType()
{
	return SORT_BYNAME;
}

void cUtil::checkLogFiles()
{
	int fileSize;
	ifstream file;
	ofstream clr;

	// Game log
	if (!core.serverMode)
	{
		file.open("Logs//game.txt");
		file.seekg(0, file.end);
		fileSize = (int)file.tellg();
		file.close();
		// Filesize is more than a megabyte
		if (fileSize > 1000000) {
			ofstream clr;
			clr.open("Logs//game.txt");
			clr.close();
		}
	}
	// Server mode
	else
	{
		file.open("Logs//server.txt");
		file.seekg(0, file.end);
		fileSize = (int)file.tellg();
		file.close();
		// Filesize is more than a megabyte
		if (fileSize > 1000000) {
			ofstream clr;
			clr.open("Logs//server.txt");
			clr.close();
		}
	}
}

bool cUtil::intersects(vec2f pos, vec2f rectPos, vec2f rectSize)
{
	if (pos.x < rectPos.x) { return false; }
	if (pos.y < rectPos.y) { return false; }
	if (pos.x > rectPos.x + rectSize.x) { return false; }
	if (pos.y > rectPos.y + rectSize.y) { return false; }
	return true;
}