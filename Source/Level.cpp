#include "Level.h"

int count(int a) {
	int length = 0;
	while (a != 0) {
		a /= 10;
		length++;
	}
	return length;
}

void Level::printGrids() {
	std::string creset = "\033[97m";
	//adds headers
	for (int i = 0; i < (gridSize * 2) + 21; i++) {
		if (i < (gridSize - 5) + leftmargin)
			std::cout << ' ';
		else if (i == (gridSize - 5) + leftmargin)
			std::cout << std::setw(11) << color << "Enemy grid" << creset;
		else if (i < (gridSize - 1) + (2 * (gridSize - 5)) + leftmargin + dividor)
			std::cout << ' ';
		else {
			std::cout << color << "Your grid\n\n" << creset;
			break;
		}
	}

	//prints the grids
	for (int i = -topmargin; i < gridSize; i++) {
		for (int j = -leftmargin; j < gridSize * 2 + leftmargin + dividor; j++) {
			if (i == -topmargin) {	//prints the ABCDEFGHIJ
				if (j < 0)
					std::cout << std::setw(14);
				else if (j < gridSize)
					std::cout << color << (char)('A' + j) << creset << ' ';
				else if (j < gridSize + leftmargin + dividor)
					std::cout << ' ';
				else
					std::cout << color << (char)('A' + ((j - (leftmargin + dividor)) % gridSize)) << creset << ' ';
			}
			else {
				if (j == 0) {

					if (count(i + 1) > 1) {
						std::cout << std::setw(11) << color << i + 1 << creset;
						std::cout << ' ';
					}
					else {
						std::cout << std::setw(11) << color << i + 1 << creset;
						std::cout << "  ";
					}
				}
				else if (j <= gridSize and j > 0) {
					if (player2.grid[i][(j - 1) % gridSize] == 'X')
						std::cout << "\033[31m" << player2.grid[i][(j - 1) % gridSize] << creset << ' ';
					else
						std::cout << player2.grid[i][(j - 1) % gridSize] << ' ';

				}
				else if (j == gridSize + 1) {
					std::cout << "  " << color << i + 1 << creset;
					if (count(i + 1) > 1)
						std::cout << ' ';
					else std::cout << "  ";
				}
				else if (j >= gridSize + leftmargin + dividor) {
					if (player1.grid[i][((j - (leftmargin + dividor)) % gridSize)] == 'X')
						std::cout << "\033[31m" << player1.grid[i][((j - (leftmargin + dividor)) % gridSize)] << creset << ' ';
					else
						std::cout << player1.grid[i][((j - (leftmargin + dividor)) % gridSize)] << ' ';
				}
			}
		}
		std::cout << std::endl;
	}
}

void Level::initializeGrids() {
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			player1.grid[i][j] = '~';
			player2.grid[i][j] = '~';
			playerCopy.grid[i][j] = '~';
		}
	}
}

int Level::checkhit(char x[gridSize][gridSize], int r, int c) {
	if (x[r][c] == 'S') {
		x[r][c] = 'X';
		return 1;
	}
	else if (x[r][c] == '~') {
		x[r][c] = 'O';
		return 0;
	}
}

void Level::coolprinting(std::string s) {
	char* array = &s[0];
	for (std::string::size_type i = 0; i < s.length(); i++) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		std::cout << array[i];
	}
}

void Level::placeShips() {
	placeShips(Carrier, player1.grid);
	placeShips(Battleship, player1.grid);
	placeShips(Cruiser, player1.grid);
	placeShips(Submarine, player1.grid);
	placeShips(Destroyer, player1.grid);

	placeShips(Carrier, playerCopy.grid);
	placeShips(Battleship, playerCopy.grid);
	placeShips(Cruiser, playerCopy.grid);
	placeShips(Submarine, playerCopy.grid);
	placeShips(Destroyer, playerCopy.grid);
}

void Level::placeShips(ships s, char x[gridSize][gridSize]) {
	int r, c = 0;
	bool consecutive = false;
	int rotation = rand() % 2;	//0 = vertical || 1 = horizontal
	//randomly select a row and a column and check its validity
	if (rotation) {
		//horizontal
		while (!consecutive) {
			r = rand() % gridSize;
			c = rand() % (gridSize - s);
			for (int i = -1; i <= s; i++) {
				consecutive = true;
				if (c == 0 && i == -1)
					i++;
				if (x[r][c + i] != '~') {
					consecutive = false;
					break;
				}
				if ((r - 1 > 0) ? x[r - 1][c + i] != '~' : true) {
					consecutive = false;
					break;
				}
				if ((r + 1 < gridSize) ? x[r + 1][c + i] != '~' : true) {
					consecutive = false;
					break;
				}
			}
		}
		for (int i = 0; i < s; i++)
			x[r][c + i] = 'S';
	}
	else {
		//vertical
		while (!consecutive) {
			r = rand() % (gridSize - s);
			c = rand() % gridSize;
			for (int i = -1; i <= s; i++) {
				consecutive = true;
				if (r == 0 && i == -1)
					i++;
				if (x[r + i][c] != '~') {
					consecutive = false;
					break;
				}
				if ((c - 1 > 0) ? x[r + i][c - 1] != '~' : true) {
					consecutive = false;
					break;
				}
				if ((c + 1 < gridSize) ? x[r + i][c + 1] != '~' : true) {
					consecutive = false;
					break;
				}
			}
		}
		for (int i = 0; i < s; i++)
			x[r + i][c] = 'S';
	}
}