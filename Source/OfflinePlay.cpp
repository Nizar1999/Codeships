#include "OfflinePlay.h"

void OfflinePlay::beginPlay() {
	playerCopy = player2;
game:
	initializeGrids();
	char coordinates[3];
	std::string dialog;
	bool turn = 0;
	bool alive1 = false;
	bool alive2 = false;
	int oldr, oldc;
	int flag = 0, stage = 0;
	int initialr = 0, initialc = 0;

	//Load the grids
	placeShips();
	system("cls");
	printGrids();

	dialog = "\n... The tides of war rage on.";
	coolprinting(dialog);
	do {
		int r = 0, c = 0;
		alive1 = false;
		alive2 = false;
		turn = !turn;
		//Player turn
		if (turn) {
			std::cout << "\nEnter coordinates to launch an attack." << std::endl;
			try {
				while (1) {
					Console::getInstance().showConsoleCursor(true);
					std::cin >> coordinates;
					Console::getInstance().showConsoleCursor(false);
					//Validate input
					if (std::isdigit(coordinates[0]) && std::isalpha(coordinates[1]) && coordinates[2] == NULL) {
						if (coordinates[0] > '0') {
							r = coordinates[0] - '0' - 1;
							if (std::isupper(coordinates[1]) && coordinates[1] <= 'J') {
								c = (int)coordinates[1] % 'A';
								break;
							}
							else if (std::islower(coordinates[1]) && coordinates[1] <= 'j') {
								c = (int)coordinates[1] % 'a';
								break;
							}
						}
					}
					else if (coordinates[0] == '1' && coordinates[1] == '0' && std::isalpha(coordinates[2])) {
						if (coordinates[2] <= 'J' || coordinates[2] <= 'j') {
							r = ((coordinates[0] - '0') * 10 + (coordinates[1] - '0')) - 1;
							if (std::isupper(coordinates[2]) && coordinates[2] <= 'J') {
								c = (int)coordinates[2] % 'A';
								break;
							}
							else if (std::islower(coordinates[2]) && coordinates[2] <= 'j') {
								c = (int)coordinates[2] % 'a';
								break;
							}
						}
					}
					std::cout << "Invalid coordinates.\n";
				}
			}
			catch (std::exception e) {
				std::cout << "Invalid coordinates.";
			}
			dialog = "\nLaunching rockets..";
			coolprinting(dialog);
			if (playerCopy.grid[r][c] == 'S') {
				playerCopy.grid[r][c] = 'X';
				player2.grid[r][c] = 'X';
				std::cout << "\nHit!" << std::endl;
			}
			else if (player2.grid[r][c] == '~') {
				player2.grid[r][c] = 'O';
				std::cout << "\nMissed!" << std::endl;
			}
		}
		else {
		generate_random:
			if (stage == 0) {				//Randomly throw missiles in search for ships
				r = rand() % gridSize;		//generate random coordinates
				c = rand() % gridSize;

				//check if coordinates have an X 1 box away
				if (!(r + 1 == gridSize || r + 1 < 0 || c == gridSize || c < 0)) {
					if (player1.grid[r + 1][c] == 'X' || player1.grid[r + 1][c] == 'O') {
						goto generate_random;
					}
				}
				if (!(r - 1 == gridSize || r - 1 < 0 || c == gridSize || c < 0)) {
					if (player1.grid[r - 1][c] == 'X' || player1.grid[r - 1][c] == 'O') {
						goto generate_random;
					}
				}
				if (!(r == gridSize || r < 0 || c + 1 == gridSize || c + 1 < 0)) {
					if (player1.grid[r][c + 1] == 'X' || player1.grid[r][c + 1] == 'O') {
						goto generate_random;
					}
				}
				if (!(r == gridSize || r < 0 || c - 1 == gridSize || c - 1 < 0)) {
					if (player1.grid[r][c - 1] == 'X' || player1.grid[r][c - 1] == 'O') {
						goto generate_random;
					}
				}

				oldr = r;					//save them
				oldc = c;
				if (player1.grid[r][c] == 'S') {
					stage = 1;
				}
			}
			else if (stage == 1) {			//If ship has been found, look for the direction
				//Try out directions
				r = oldr;
				c = oldc;


				//Flag Legend: 0-S | 1-N | 2-E | 3-W
			f2:
				switch (flag) {
				case 0:
					r = oldr + 1;
					c = oldc;
					break;
				case 1:
					r = oldr - 1;
					c = oldc;
					break;
				case 2:
					c = oldc + 1;
					r = oldr;
					break;
				case 3:
					c = oldc - 1;
					r = oldr;
					break;
				}

				//check if direction is out of bounds
				if (r == gridSize || r < 0 || c == gridSize || c < 0) {
					flag = (flag + 1) % 4;
					goto f2;
				}

				//if direction yields a hit, commit to stage 2
				if (player1.grid[r][c] == 'S') {
					stage = 2;
					initialr = oldr;
					initialc = oldc;
					oldr = r;
					oldc = c;
				}

				//If direction yields a miss, try for a new direction
				if (player1.grid[r][c] == '~' || player1.grid[r][c] == 'O') {
					flag++;
				}
			}
			else if (stage == 2 || stage == 3) {	//direction has been confirmed, save the coordinates of that box and commit in the direction till you reach a miss. Then persist in the opp direction
				r = oldr;
				c = oldc;

				switch (flag) {
				case 0:			//S
					r = oldr + 1;
					c = oldc;
					break;
				case 1:			//N
					r = oldr - 1;
					c = oldc;
					break;
				case 2:			//E
					c = oldc + 1;
					r = oldr;
					break;
				case 3:			//W
					c = oldc - 1;
					r = oldr;
					break;
				}
				if (r == gridSize || r < 0 || c == gridSize || c < 0) {
					flag = (flag + 1) % 4;
					goto f2;
				}

				if (player1.grid[r][c] == 'S') { //if yields to hit, proceed in that direction
					oldr = r;
					oldc = c;
				}

				//If direction yields a miss, reset to the original X and go in the opp direction
				if (player1.grid[r][c] == '~' || player1.grid[r][c] == 'O' || player1.grid[r][c] == 'X') {
					oldr = initialr;
					oldc = initialc;

					if (flag == 0 || flag == 2)
						flag++;
					else {
						flag = (flag + 3) % 4;
					}

					stage++;   //if it hits stage 4 then we've tried both directions and we're done
				}


			}
			else if (stage == 4) {
				flag = 0; //reset direction flag
				stage = 0; //reset stage
				goto generate_random;
			}
			dialog = "\nEnemy is attacking! ";
			coolprinting(dialog);
			int result = checkhit(player1.grid, r, c);
			if (result)
				std::cout << "\nHit!" << std::endl;
			else
				std::cout << "\nMissed!" << std::endl;
		}

		//check if game is going
		for (int i = 0; i < gridSize; i++) {
			for (int j = 0; j < gridSize; j++) {
				if (player1.grid[i][j] == 'S')
					alive1 = true;
				else if (playerCopy.grid[i][j] == 'S')
					alive2 = true;
			}
		}

		if (!alive1) {
			dialog = "\nThe enemies are victorious, comrade.";
			coolprinting(dialog);
		}
		if (!alive2) {
			dialog = "\nWe are victorious! Rejoice, general.";
			coolprinting(dialog);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		system("cls");
		printGrids();
	} while (alive1 && alive2);

	std::cout << std::endl << std::endl;
	std::cout << std::setw(35) << "- Restart -" << std::endl;
	std::cout << std::setw(34) << "- Quit -" << std::endl;
	
	initializeGrids();
	beginPlay();

	char key;

	while (1) {
		key = _getch();
		if (key == 'r' || key == 'R') {
			beginPlay();
		}
		if (key == 'q' || key == 'Q') {
			break;
		}
	}
}
