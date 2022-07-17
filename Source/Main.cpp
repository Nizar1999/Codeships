#include "Console.h"
#include "MainMenu.h"
#include <string>
#include <iostream>

int main() {
	//PlaySound(L"BGTrack.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
	srand((unsigned int)time(NULL));
	Console& newWindow = Console::getInstance();
	newWindow.initConsole({530, 600});
	Player* player1 = new Player();
	Player* player2 = new Player();
	
	MainMenu mainMenu;
	mainMenu.player1 = *player1;
	mainMenu.player2 = *player2;
	mainMenu.beginPlay();
	
	return 0;
}