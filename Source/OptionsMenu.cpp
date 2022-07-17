#include "OptionsMenu.h"

void OptionsMenu::beginPlay() {
	system("color 3F");
	system("cls");
	std::cout << logo;
	initializeGrids();
	printGrids();
	std::cout << std::endl;
	std::cout << std::setw(30) << color << "- Play -" << std::endl;
	std::cout << std::setw(30) << color << "- Quit -" << std::endl;
	char key;
	while (1) {
		key = _getch();
		if (key == 'p' || key == 'P') {
			ModeMenu modeMenu;
			modeMenu.player1 = player1;
			modeMenu.player2 = player2;
			modeMenu.prevLevel = this;
			modeMenu.beginPlay();
			break;
		}
		if (key == 'q' || key == 'Q') {
			break;
		}
	}
}