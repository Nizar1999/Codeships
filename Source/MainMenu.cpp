#include "MainMenu.h"

void MainMenu::beginPlay() {
	bool x = false;
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j += 2) {
			player1.grid[i][j] = '~';
			player2.grid[i][j] = '~';
		}
	}
	while (!_kbhit()) {
		system("color 3F");
		std::cout << logo;
		printGrids();
		std::cout << std::endl;
		std::cout << std::setw(83);
		if (x)
			std::cout << std::setw(23) << color << "CLICK ANY KEY TO CONTINUE...\033[96m";
		x = !x;
		for (int i = 0; i < gridSize; i++) {
			for (int j = 0; j < gridSize; j++) {
				if (player1.grid[i][j] == '~')
					player1.grid[i][j] = '-';
				else
					player1.grid[i][j] = '~';
				if (player2.grid[i][j] == '~')
					player2.grid[i][j] = '-';
				else
					player2.grid[i][j] = '~';
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(600));
		system("cls");
	}
	char flush = _getch();
	OptionsMenu optionsMenu;
	optionsMenu.player1 = player1;
	optionsMenu.player2 = player2;
	optionsMenu.beginPlay();
}
