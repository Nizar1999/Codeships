#pragma once

#include <atlbase.h>
#include <atlwin.h>
#include <Winuser.h>

class Console {
private:
	HWND consoleWindow;
	SIZE consoleSize;
	SIZE consolePos;

public:
	int columns, rows;
	bool initConsole(SIZE);			//Initialize and create console window
	bool getMonitorSize(SIZE*);		//Retreive size of current monitor
	bool setPosAndSize(SIZE);		//Set console to center
	bool setFont();				//Set font to consolas-16
	bool showConsoleCursor(bool);		//Toggle cursor visibility

	void MoveCursor(SHORT, SHORT);

	static Console& getInstance() {
		static Console instance;
		return instance;
	}

	Console(Console const&) = delete;
	void operator=(Console const&) = delete;

private:
	Console() {}
};