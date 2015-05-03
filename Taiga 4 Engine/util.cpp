
#include "main.h"

void cUtil::detectVideoCard()
{
	string vendor = reinterpret_cast< char const * >(glGetString(GL_VENDOR));
	if (vendor == "Intel") {
		core.videoCard = VIDEO_INTEL;
		cout << "[MAIN] Detected Intel video card" << "\n";
	}
	else if (vendor.substr(0, 6) == "NVIDIA") {
		core.videoCard = VIDEO_NVIDIA;
		cout << "[MAIN] Detected Nvidia video card" << "\n";
	}
	else if (vendor.substr(0, 3) == "ATI") { 
		core.videoCard = VIDEO_ATI;
		cout << "[MAIN] Detected AMD video card" << "\n";
	}
	else if (vendor.substr(0, 9) == "Microsoft") {
		core.videoCard = VIDEO_MICROSOFT;
		cout << "[MAIN] Detected Microsoft video card" << "\n";
	}
	else {
		core.videoCard = VIDEO_OTHER;
		cout << "[MAIN] Unknown video card: " << vendor << "\n";
	}
}