
#ifdef _WIN32
	#define SYSTEM_WINDOWS
#elif defined __ANDROID__
	#define SYSTEM_ANDROID
#elif defined __linux__
	#define SYSTEM_LINUX
#else
	#define SYSTEM_UNKNOWN
#endif

#include <iostream>
#include <regex>
#include <string>
#include <functional>
#include <thread>
#include <sstream>
#include <fstream>
#include <locale>
#include <ctime>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>

#ifdef SYSTEM_WINDOWS
	#include <Windows.h>
#endif