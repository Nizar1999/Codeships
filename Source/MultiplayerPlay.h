#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <winuser.h>
#include <Windows.h>
#include "Level.h"
#pragma comment(lib, "ws2_32.lib") //Winsock Library
#pragma comment(lib, "user32.lib")

class MultiplayerPlay : public Level
{
	int initConnection(std::string, SOCKET&);
	int initServer(SOCKET&);
public:
	bool status;
	void beginPlay() override;
};

