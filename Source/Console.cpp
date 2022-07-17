/**
 * Responsible for creation and
 * management of a console window
 */

#include "Console.h"

bool Console::initConsole(SIZE windowSize) {
	consoleSize = windowSize;
	consoleWindow = GetConsoleWindow();

	RECT ConsoleRect;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SIZE monitorSize;
	COORD NewSBSize{62,35};

	if (setFont())
		if(getMonitorSize(&monitorSize))
			if(setPosAndSize(monitorSize))
				if (SetConsoleScreenBufferSize(hOut, NewSBSize))
					if (showConsoleCursor(false))
				return 1;
	return 0;
}

bool Console::getMonitorSize(SIZE* monitorSize) {
	HMONITOR monitor = MonitorFromWindow(consoleWindow, MONITOR_DEFAULTTONEAREST);  //Get handle to monitor
	MONITORINFO info;													//Declare object to hold monitor info
	info.cbSize = sizeof(MONITORINFO);										//Initialize cbsize
	if (GetMonitorInfo(monitor, &info)) {									//Retreive monitor info and store it in info object
		monitorSize->cx = info.rcMonitor.right - info.rcMonitor.left;			//Set the monitor width
		monitorSize->cy = info.rcMonitor.bottom - info.rcMonitor.top;			//Set the monitor height
		return 1;														//success
	}
	return 0;
}

bool Console::setPosAndSize(SIZE monitorSize) {
	consolePos.cx = monitorSize.cx / 2 - consoleSize.cx / 2;
	consolePos.cy = monitorSize.cy / 2 - consoleSize.cy / 2;

	DWORD style = GetWindowLong(consoleWindow, GWL_STYLE);
	style &= ~WS_MAXIMIZEBOX;
	SetWindowLong(consoleWindow, GWL_STYLE, style);
	return SetWindowPos(consoleWindow, 0, consolePos.cx, consolePos.cy, consoleSize.cx, consoleSize.cy, SWP_SHOWWINDOW | SWP_FRAMECHANGED); //Change window position and size
}

bool Console::setFont() {
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 16;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");
	return SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

bool Console::showConsoleCursor(bool flag) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = flag;
	return SetConsoleCursorInfo(out, &cursorInfo);
}

void Console::MoveCursor(SHORT x, SHORT y) {
	COORD pos = { x, y };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
}