#include "ModeMenu.h"

void ModeMenu::beginPlay() {
	system("color 3F");
	system("cls");
	std::cout << logo;
	initializeGrids();
	printGrids();
	std::cout << std::endl;
	std::cout << std::setw(29) << color << "- Offline -" << std::endl;
	std::cout << std::setw(27) << color << "- Multiplayer -" << std::endl;
	std::cout << std::setw(30) << color << "- Back -" << std::endl;
	char key;
	while (1) {
		key = _getch();
		if (key == 'o' || key == 'O') {
			OfflinePlay offlinePlay;
			offlinePlay.player1 = player1;
			offlinePlay.player2 = player2;
			offlinePlay.beginPlay();
			break;
		}
		if (key == 'm' || key == 'M') {
			MultiplayerMenu multiplayerMenu;
			multiplayerMenu.player1 = player1;
			multiplayerMenu.player2 = player2;
			multiplayerMenu.prevLevel = this;
			multiplayerMenu.beginPlay();
			break;
		}
		if (key == 'b' || key == 'B') {
			prevLevel->beginPlay();
		}
	};
}