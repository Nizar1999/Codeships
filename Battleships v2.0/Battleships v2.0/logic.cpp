#include "stdafx.h"
#include "logic.h"
#include "graphics.h"

std::string color = "\033[33m";
char yourgrid[gridsize][gridsize];
char enemygrid[gridsize][gridsize];
char enemygridcopy[gridsize][gridsize];
const int leftmargin = 3;
const int topmargin = 1;
const int dividor = 2;
std::string logo = R"(
______  ___ _____ _____ _      _____ _____ _   _ ___________ 
| ___ \/ _ \_   _|_   _| |    |  ___/  ___| | | |_   _| ___ \
| |_/ / /_\ \| |   | | | |    | |__ \ `--.| |_| | | | | |_/ /
| ___ \  _  || |   | | | |    |  __| `--. \  _  | | | |  __/ 
| |_/ / | | || |   | | | |____| |___/\__/ / | | |_| |_| |    
\____/\_| |_/\_/   \_/ \_____/\____/\____/\_| |_/\___/\_|                                                             
                                                                    
______________________________________________________________


)";


//Game sequence functions

void intro() {
	PlaySound(L"the_river.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
	bool x = false;
	for (int i = 0; i < gridsize; i++) {
		for (int j = 0; j < gridsize; j += 2) {
			yourgrid[i][j] = '~';
			enemygridcopy[i][j] = '-';
		}
	}
	while (!_kbhit()) {
		system("color 0B");
		cout << logo;
		printgrids();
		cout << endl;
		cout << std::setw(83);
		if (x)
			std::cout << color << "CLICK ANY KEY TO CONTINUE...\033[96m";
		x = !x;
		for (int i = 0; i < gridsize; i++) {
			for (int j = 0; j < gridsize; j++) {
				if (yourgrid[i][j] == '~')
					yourgrid[i][j] = '-';
				else
					yourgrid[i][j] = '~';
				if (enemygridcopy[i][j] == '~')
					enemygridcopy[i][j] = '-';
				else
					enemygridcopy[i][j] = '~';
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(600));
		system("cls");
	}
	char flush = _getch();
	menu();
}

void menu() {
	system("cls");
	cout << logo;
	initializegrid();
	printgrids();
	cout << endl;
	cout << std::setw(30) << color << "- Play -" << endl;
	cout << std::setw(30) << color << "- Quit -" << endl;
	char key;
	while (1) {
		key = _getch();
		if (key == 'p' || key == 'P') {
			menu2();
		}
		if (key == 'q' || key == 'Q') {
			break;
		}
	}
}

void menu2() {
	system("cls");
	cout << logo;
	printgrids();
	cout << endl;
	cout << std::setw(29) << color << "- Offline -" << endl;
	cout << std::setw(27) << color << "- Multiplayer -" << endl;
	cout << std::setw(30) << color << "- Back -" << endl;
	char key;
	while (1) {
		key = _getch();
		if (key == 'o' || key == 'O') {
			offlineGameplay();
			break;
		}
		if (key == 'm' || key == 'M') {
			multiplayerMenu();
			break;
		}
		if (key == 'b' || key == 'B') {
			menu();
		}
	};
}

void multiplayerMenu() {
	system("cls");
	cout << logo;
	printgrids();
	cout << endl;
	cout << std::setw(28) << color << "- Host Game -" << endl;
	cout << std::setw(28) << color << "- Join Game -" << endl;
	cout << std::setw(30) << color << "- Back -" << endl;
	char key;
	while (1) {
		key = _getch();
		if (key == 'h' || key == 'H') {
			multiplayerGameplay(1);
			break;
		}
		if (key == 'j' || key == 'J') {
			multiplayerGameplay(0);
			break;
		}
		if (key == 'b' || key == 'B') {
			menu2();
		}
	};
}

//Game logic functions

void offlineGameplay() {
game:
	char coordinates[3];
	string dialog;
	bool turn = 0;
	bool alive1 = false;
	bool alive2 = false;
	int oldr, oldc;
	int flag = 0, stage = 0;
	int initialr = 0, initialc = 0;

	//Load the grids
	place_ships();
	system("cls");
	printgrids();

	dialog = "\n... The tides of war rage on.";
	coolprinting(dialog);
	do {
		int r = 0, c = 0;
		alive1 = false;
		alive2 = false;
		turn = !turn;
		//Player turn
		if (turn) {
			cout << "\nEnter coordinates to launch an attack." << endl;
			try {
				while (1) {
					showConsoleCursor(true);
					cin >> coordinates;
					showConsoleCursor(false);
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
					cout << "Invalid coordinates.\n";
				}
			}
			catch (std::exception e) {
				cout << "Invalid coordinates.";
			}
			dialog = "\nLaunching rockets..";
			coolprinting(dialog);
			if (enemygrid[r][c] == 'S') {
				enemygrid[r][c] = 'X';
				enemygridcopy[r][c] = 'X';
				cout << "\nHit!" << endl;
			}
			else if (enemygridcopy[r][c] == '~') {
				enemygridcopy[r][c] = 'O';
				cout << "\nMissed!" << endl;
			}
		}
		else {
		generate_random:
			if (stage == 0) {				//Randomly throw missiles in search for ships
				r = rand() % gridsize;		//generate random coordinates
				c = rand() % gridsize;

				//check if coordinates have an X 1 box away
				if (!(r + 1 == gridsize || r + 1 < 0 || c == gridsize || c < 0)) {
					if (yourgrid[r + 1][c] == 'X' || yourgrid[r + 1][c] == 'O') {
						goto generate_random;
					}
				}
				if (!(r - 1 == gridsize || r - 1 < 0 || c == gridsize || c < 0)) {
					if (yourgrid[r - 1][c] == 'X' || yourgrid[r - 1][c] == 'O') {
						goto generate_random;
					}
				}
				if (!(r == gridsize || r < 0 || c + 1 == gridsize || c + 1 < 0)) {
					if (yourgrid[r][c + 1] == 'X' || yourgrid[r][c + 1] == 'O') {
						goto generate_random;
					}
				}
				if (!(r == gridsize || r < 0 || c - 1 == gridsize || c - 1 < 0)) {
					if (yourgrid[r][c - 1] == 'X' || yourgrid[r][c - 1] == 'O') {
						goto generate_random;
					}
				}

				oldr = r;					//save them
				oldc = c;
				if (yourgrid[r][c] == 'S') {
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
				if (r == gridsize || r < 0 || c == gridsize || c < 0) {
					flag = (flag + 1) % 4;
					goto f2;
				}

				//if direction yields a hit, commit to stage 2
				if (yourgrid[r][c] == 'S') {
					stage = 2;
					initialr = oldr;
					initialc = oldc;
					oldr = r;
					oldc = c;
				}

				//If direction yields a miss, try for a new direction
				if (yourgrid[r][c] == '~' || yourgrid[r][c] == 'O') {
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
				if (r == gridsize || r < 0 || c == gridsize || c < 0) {
					flag = (flag + 1) % 4;
					goto f2;
				}

				if (yourgrid[r][c] == 'S') { //if yields to hit, proceed in that direction
					oldr = r;
					oldc = c;
				}

				//If direction yields a miss, reset to the original X and go in the opp direction
				if (yourgrid[r][c] == '~' || yourgrid[r][c] == 'O' || yourgrid[r][c] == 'X') {
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
			int result = checkhit(yourgrid, r, c);
			if (result)
				cout << "\nHit!" << endl;
			else
				cout << "\nMissed!" << endl;
		}

		//check if game is going
		for (int i = 0; i < gridsize; i++) {
			for (int j = 0; j < gridsize; j++) {
				if (yourgrid[i][j] == 'S')
					alive1 = true;
				else if (enemygrid[i][j] == 'S')
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
		printgrids();
	} while (alive1 && alive2);

	cout << endl << endl;
	cout << std::setw(35) << "- Restart -" << endl;
	cout << std::setw(34) << "- Quit -" << endl;
	char key;

	while (1) {
		key = _getch();
		if (key == 'r' || key == 'R') {
			initializegrid();
			goto game;
		}
		if (key == 'q' || key == 'Q') {
			break;
		}
	}
}

void initializegrid() {
	//initialize grid
	for (int i = 0; i < gridsize; i++) {
		for (int j = 0; j < gridsize; j++) {
			yourgrid[i][j] = '~';
			enemygrid[i][j] = '~';
			enemygridcopy[i][j] = '~';
		}
	}
}

void place_ships() {
	place_ships(Carrier, yourgrid);
	place_ships(Battleship, yourgrid);
	place_ships(Cruiser, yourgrid);
	place_ships(Submarine, yourgrid);
	place_ships(Destroyer, yourgrid);

	place_ships(Carrier, enemygrid);
	place_ships(Battleship, enemygrid);
	place_ships(Cruiser, enemygrid);
	place_ships(Submarine, enemygrid);
	place_ships(Destroyer, enemygrid);
}

void place_ships(ships s, char x[gridsize][gridsize]) {
	int r, c = 0;
	bool consecutive = false;
	int rotation = rand() % 2;	//0 = vertical || 1 = horizontal
	//randomly select a row and a column and check its validity
	if (rotation) {
		//horizontal
		while (!consecutive) {
			r = rand() % gridsize;
			c = rand() % (gridsize - s);
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
				if ((r + 1 < gridsize) ? x[r + 1][c + i] != '~' : true) {
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
			r = rand() % (gridsize - s);
			c = rand() % gridsize;
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
				if ((c + 1 < gridsize) ? x[r + i][c + 1] != '~' : true) {
					consecutive = false;
					break;
				}
			}
		}
		for (int i = 0; i < s; i++)
			x[r + i][c] = 'S';
	}
}

int checkhit(char x[gridsize][gridsize], int r, int c)
{
	if (x[r][c] == 'S') {
		x[r][c] = 'X';
		return 1;
	}
	else if (x[r][c] == '~') {
		x[r][c] = 'O';
		return 0;
	}
}

void multiplayerGameplay(bool status)
{
	//Set up connections

	SOCKET s;
	system("cls");
	cout << logo;
	printgrids();
	cout << endl;
	if (status) {	//Server
		int errorValue = initServer(s);
		if (errorValue != 0) {
			cout << std::setw(20) << color << "- Could not start server. Error: " << errorValue << ' -' << endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			multiplayerMenu();
		}
	}
	else {	//Client
		string IP;
		//Ask for IP
		cout << std::setw(28) << color << "- IP Address -" << endl;
		MoveCursor(25, 28);
		showConsoleCursor(true);
		cin >> IP;
		showConsoleCursor(false);

		//Validate input here
		if (IP.length() < 15 || IP.length() > 7) {

			int errorValue = initConnection(IP, s);

			if (errorValue != 0) {
				system("cls");
				cout << logo;
				printgrids();
				cout << endl;

				if (errorValue != -69) {
					cout << std::setw(20) << color << "- Could not connect. Error: " << errorValue << ' -' << endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					multiplayerMenu();
				}
				else {
					cout << std::setw(28) << color << "- Invalid IP -" << endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					multiplayerMenu();
				}
			}
		}
		else {
			cout << std::setw(28) << color << "- Invalid IP -" << endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			multiplayerMenu();
		}

	}

	//Connected
	
	cout << std::setw(28) << color << "- Connected -" << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	
	//Game logic

	char coordinates[3];
	string dialog;
	bool turn = !status;
	bool alive1 = false;
	bool alive2 = false;
	char hit[1];
	char win[1];

	//Load the grids
	place_ships();
	system("cls");
	printgrids();
	dialog = "\n... The tides of war rage on.";
	coolprinting(dialog);

	do {
		int r = 0, c = 0;
		alive1 = false;
		alive2 = true;
		turn = !turn;
		

		if (turn) {
			cout << "\nEnter coordinates to launch an attack." << endl;
			try {
				while (1) {
					showConsoleCursor(true);
					cin >> coordinates;
					showConsoleCursor(false);
					//Validate input
					if (std::isdigit(coordinates[0]) && std::isalpha(coordinates[1]) && coordinates[2] == NULL) {
						if (coordinates[0] > '0') {
							coordinates[1] = std::tolower(coordinates[1]);
							if (coordinates[1] >= 'a' && coordinates[1] <= 'j') {
								r = coordinates[0] - '0' - 1;
								c = (int)coordinates[1] % 'a';
								break;
							}
						}
					}
					else if (coordinates[0] == '1' && coordinates[1] == '0' && std::isalpha(coordinates[2])) {
						coordinates[2] = std::tolower(coordinates[2]);
						if (coordinates[2] >= 'a' && coordinates[2] <= 'j') {
							r = ((coordinates[0] - '0') * 10 + (coordinates[1] - '0')) - 1;
							c = (int)coordinates[2] % 'a';
							break;
						}
					}
					cout << "Invalid coordinates.\n";
				}
			}
			catch (std::exception e) {
				cout << "Invalid coordinates.";
			}
			dialog = "\nLaunching rockets..";
			coolprinting(dialog);
			//Send coordinates
			if (send(s, coordinates, sizeof(coordinates), 0) < 0) {
				cout << "Player disconnected. " << endl;
				multiplayerMenu();
			}
			//Go into recieving mode
			int recv_size = recv(s, hit, sizeof(hit), 0);
			//Update grid
			if (hit[0] == '1') {
				enemygrid[r][c] = 'X';
				enemygridcopy[r][c] = 'X';
				cout << "\nHit!" << endl;
			}
			else {
				enemygridcopy[r][c] = 'O';
				cout << "\nMissed!" << endl;
			}

			//check if u won
			recv_size = recv(s, win, sizeof(win), 0);
			if (win[0] == '1')
				alive2 = false;

			if (!alive2) {
				dialog = "\nWe are victorious! Rejoice, general.";
				coolprinting(dialog);
			}

			//check if game is going
			for (int i = 0; i < gridsize; i++) {
				for (int j = 0; j < gridsize; j++) {
					if (yourgrid[i][j] == 'S')
						alive1 = true;
				}
			}
		}
		else {
			memset(coordinates, 0, sizeof(coordinates)); //clear coordinates
			//Not my turn
			dialog = "\nEnemy is attacking! ";
			coolprinting(dialog);
			//Go into recieving mode

		    int recv_size = recv(s, coordinates, sizeof(coordinates), 0);
			cout << "\nCoordinates: " << coordinates[0] << coordinates[1] << coordinates[2] << endl;
			//Check if it hits and update grid
			
			r = coordinates[0] - '0' - 1;
			c = (int)coordinates[1] % 'a';

			if (coordinates[2] != NULL) {
				c = (int)coordinates[2] % 'a';
				r = ((coordinates[0] - '0') * 10 + (coordinates[1] - '0')) - 1;
			}
			
			int result = checkhit(yourgrid, r, c);

			if (result) {
				cout << "\nHit!" << endl;
				hit[0] = '1';
			}
			else {
				cout << "\nMissed!" << endl;
				hit[0] = '0';
			}

			//Send result
			if (send(s, hit, sizeof(hit), 0) < 0) {
				cout << "Player disconnected. " << endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(900));
				multiplayerMenu();
			}

			//check if game is going
			for (int i = 0; i < gridsize; i++) {
				for (int j = 0; j < gridsize; j++) {
					if (yourgrid[i][j] == 'S')
						alive1 = true;
				}
			}

			if (!alive1) {
				dialog = "\nThe enemies are victorious, comrade.";
				coolprinting(dialog);
				//send win
				win[0] = '1';
				if (send(s, win, sizeof(win), 0) < 0) {
					cout << "Player disconnected. " << endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(900));
					multiplayerMenu();
				}
			}
			else {
				win[0] = '0';
				if (send(s, win, sizeof(win), 0) < 0) {
					cout << "Player disconnected. " << endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(900));
					multiplayerMenu();
				}
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		system("cls");
		printgrids();
	} while (alive1 && alive2);
	closesocket(s);
	WSACleanup();
	multiplayerMenu();
}

//Utility Functions

void printgrids() {
	string creset = "\033[96m";
	//adds headers
	for (int i = 0; i < (gridsize * 2) + 21; i++) {
		if (i < (gridsize - 5) + leftmargin)
			cout << ' ';
		else if (i == (gridsize - 5) + leftmargin)
			cout << std::setw(11) << color << "Enemy grid" << creset;
		else if (i < (gridsize - 1) + (2 * (gridsize - 5)) + leftmargin + dividor)
			cout << ' ';
		else {
			cout << color << "Your grid\n\n" << creset;
			break;
		}
	}

	//prints the grids
	for (int i = -topmargin; i < gridsize; i++) {
		for (int j = -leftmargin; j < gridsize * 2 + leftmargin + dividor; j++) {
			if (i == -topmargin) {	//prints the ABCDEFGHIJ
				if (j < 0)
					cout << std::setw(14);
				else if (j < gridsize)
					cout << color << (char)('A' + j) << creset << ' ';
				else if (j < gridsize + leftmargin + dividor)
					cout << ' ';
				else
					cout << color << (char)('A' + ((j - (leftmargin + dividor)) % gridsize)) << creset << ' ';
			}
			else {
				if (j == 0) {

					if (count(i + 1) > 1) {
						cout << std::setw(11) << color << i + 1 << creset;
						cout << ' ';
					}
					else {
						cout << std::setw(11) << color << i + 1 << creset;
						cout << "  ";
					}
				}
				else if (j <= gridsize and j > 0) {
					if (enemygridcopy[i][(j - 1) % gridsize] == 'X')
						cout << "\033[91;1m" << enemygridcopy[i][(j - 1) % gridsize] << "\033[0m" << creset << ' ';
					else
						cout << enemygridcopy[i][(j - 1) % gridsize] << ' ';

				}
				else if (j == gridsize + 1) {
					cout << "  " << color << i + 1 << creset;
					if (count(i + 1) > 1)
						cout << ' ';
					else cout << "  ";
				}
				else if (j >= gridsize + leftmargin + dividor) {
					if (yourgrid[i][((j - (leftmargin + dividor)) % gridsize)] == 'X')
						cout << "\033[91;1m" << yourgrid[i][((j - (leftmargin + dividor)) % gridsize)] << "\033[0m" << creset << ' ';
					else
						cout << yourgrid[i][((j - (leftmargin + dividor)) % gridsize)] << ' ';
				}
			}
		}
		cout << endl;
	}
}

int count(int a) {
	int length = 0;
	while (a != 0) {
		a /= 10;
		length++;
	}
	return length;
}

void coolprinting(string s) {
	char* array = &s[0];
	for (std::string::size_type i = 0; i < s.length(); i++) {
		std::this_thread::sleep_for(std::chrono::milliseconds(90));
		cout << array[i];
	}
}

void MoveCursor(SHORT x, SHORT y) {
	COORD pos = { x, y };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
}

int initConnection(string IP, SOCKET& sock) {
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;

	cout << std::setw(20) << color << "- Initializing Connection.. -" << endl;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return WSAGetLastError();

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		return WSAGetLastError();

	if (inet_addr(IP.c_str()) == INADDR_NONE)
		return -69;

	server.sin_addr.s_addr = inet_addr(IP.c_str());
	server.sin_family = AF_INET;
	server.sin_port = htons(31415);

	if (connect(s, (struct sockaddr*) & server, sizeof(server)) < 0)
		return WSAGetLastError();

	sock = s;
	return 0;
}

int initServer(SOCKET& sock) {
	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	int c;
	char data[2000];

	cout << std::setw(24) << color << "- Initializing Server.. -" << endl;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return WSAGetLastError();

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		return WSAGetLastError();

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(31415);

	//Bind
	if (bind(s, (struct sockaddr*) & server, sizeof(server)) == SOCKET_ERROR)
		return WSAGetLastError();

	listen(s, 1);

	system("cls");
	cout << logo;
	printgrids();
	cout << endl;

	cout << std::setw(19) << color << "- Waiting for players to join.. -" << endl;

	c = sizeof(struct sockaddr_in);
	new_socket = accept(s, (struct sockaddr*) & client, &c);
	
	if (new_socket == INVALID_SOCKET)
		return WSAGetLastError();

	sock = new_socket;
	return 0;
}
