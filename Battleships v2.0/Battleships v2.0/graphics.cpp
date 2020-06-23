#include "stdafx.h"
#include "graphics.h"

void initializeConsole() {
	setFont();

	//Change window size and buffer
	HANDLE hOut;
	COORD NewSBSize;
	HWND console = GetConsoleWindow();
	RECT ConsoleRect;

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//Set window size
	GetWindowRect(console, &ConsoleRect);
	MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 530, 600, TRUE);

	//Set buffer size
	NewSBSize.X = 62;
	NewSBSize.Y = 35;
	SetConsoleScreenBufferSize(hOut, NewSBSize);
	showConsoleCursor(false);
}

void setFont() {
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 16;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void showConsoleCursor(bool flag) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = flag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}