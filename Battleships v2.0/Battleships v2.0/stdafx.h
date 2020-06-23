#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <string>
#include <iostream>
#include <wchar.h>
#include <winsock2.h>
#include <winuser.h>
#include <Windows.h>
#include <iomanip>
#include <cstring>
#include <conio.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib") //Winsock Library
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "winmm.lib") //Playsound library

using std::cout;
using std::cin;
using std::endl;
using std::string;