#include "MultiplayerMenu.h"

void MultiplayerMenu::beginPlay() {
	system("cls");
	std::cout << logo;
	printGrids();
	std::cout << std::endl;
	std::cout << std::setw(28) << color << "- Host Game -" << std::endl;
	std::cout << std::setw(28) << color << "- Join Game -" << std::endl;
	std::cout << std::setw(30) << color << "- Back -" << std::endl;
	char key;
	while (1) {
		key = _getch();
		if (key == 'h' || key == 'H') {
			MultiplayerPlay multiplayerPlay;
			multiplayerPlay.player1 = player1;
			multiplayerPlay.player2 = player2;
			multiplayerPlay.status = 1;
			multiplayerPlay.prevLevel = this;
			multiplayerPlay.beginPlay();
			break;
		}
		if (key == 'j' || key == 'J') {
			MultiplayerPlay multiplayerPlay;
			multiplayerPlay.player1 = player1;
			multiplayerPlay.player2 = player2;
			multiplayerPlay.status = 0;
			multiplayerPlay.prevLevel = this;
			multiplayerPlay.beginPlay();
			break;
		}
		if (key == 'b' || key == 'B') {
			prevLevel->beginPlay();
		}
	};
}