
#include "main.h"

void cUtil::detectVideoCard()
{
	string vendor = reinterpret_cast< char const * >(glGetString(GL_VENDOR));
	if (vendor == "Intel") {
		core.videoCard = VIDEO_INTEL;
		cout << "[MAIN] Detected Intel video card" << endl;
	}
	else if (vendor == "Nvidia") {
		core.videoCard = VIDEO_NVIDIA;
		cout << "[MAIN] Detected Nvidia video card" << endl;
	}
	else if (vendor == "AMD") { 
		core.videoCard = VIDEO_AMD;
		cout << "[MAIN] Detected AMD video card" << endl;
	}
	else {
		core.videoCard = VIDEO_OTHER;
		cout << "[MAIN] Unknown video card: \"" << vendor << "\"." << endl;
	}
}