#include "MultiplayerPlay.h"

int MultiplayerPlay::initConnection(std::string IP, SOCKET& sock) {
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;

	std::cout << std::setw(20) << color << "- Initializing Connection.. -" << std::endl;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return WSAGetLastError();

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		return WSAGetLastError();

	if (inet_addr(IP.c_str()) == INADDR_NONE)
		return -69;

	server.sin_addr.s_addr = inet_addr(IP.c_str());
	server.sin_family = AF_INET;
	server.sin_port = htons(31415);

	if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0)
		return WSAGetLastError();

	sock = s;
	return 0;
}

int MultiplayerPlay::initServer(SOCKET& sock) {
	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	int c;
	char data[2000];

	std::cout << std::setw(24) << color << "- Initializing Server.. -" << std::endl;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return WSAGetLastError();

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		return WSAGetLastError();

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(31415);

	//Bind
	if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
		return WSAGetLastError();

	listen(s, 1);

	system("cls");
	std::cout << logo;
	printGrids();
	std::cout << std::endl;

	std::cout << std::setw(19) << color << "- Waiting for players to join.. -" << std::endl;

	c = sizeof(struct sockaddr_in);
	new_socket = accept(s, (struct sockaddr*)&client, &c);

	if (new_socket == INVALID_SOCKET)
		return WSAGetLastError();

	sock = new_socket;
	return 0;
}

void MultiplayerPlay::beginPlay() {
	//Set up connections

	SOCKET s;
	system("cls");
	std::cout << logo;
	printGrids();
	std::cout << std::endl;
	if (status) {	//Server
		int errorValue = initServer(s);
		if (errorValue != 0) {
			std::cout << std::setw(20) << color << "- Could not start server. Error: " << errorValue << ' -' << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			prevLevel->beginPlay();
		}
	}
	else {	//Client
		std::string IP;
		//Ask for IP
		std::cout << std::setw(28) << color << "- IP Address -" << std::endl;
		Console::getInstance().MoveCursor(25, 30);
		Console::getInstance().showConsoleCursor(true);
		std::cin >> IP;
		Console::getInstance().showConsoleCursor(false);

		//Validate input here
		if (IP.length() < 15 || IP.length() > 7) {

			int errorValue = initConnection(IP, s);

			if (errorValue != 0) {
				system("cls");
				std::cout << logo;
				printGrids();
				std::cout << std::endl;

				if (errorValue != -69) {
					std::cout << std::setw(20) << color << "- Could not connect. Error: " << errorValue << ' -' << std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					prevLevel->beginPlay();
				}
				else {
					std::cout << std::setw(28) << color << "- Invalid IP -" << std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					prevLevel->beginPlay();
				}
			}
		}
		else {
			std::cout << std::setw(28) << color << "- Invalid IP -" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			prevLevel->beginPlay();
		}

	}

	//Connected
	
	std::cout << std::setw(28) << color << "- Connected -" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	
	//Game logic

	char coordinates[3];
	std::string dialog;
	bool turn = !status;
	bool alive1 = false;
	bool alive2 = false;
	char hit[1];
	char win[1];

	playerCopy = player2;
	initializeGrids();

	//Load the grids
	placeShips();
	system("cls");
	printGrids();
	dialog = "\n... The tides of war rage on.";
	coolprinting(dialog);

	do {
		int r = 0, c = 0;
		alive1 = false;
		alive2 = true;
		turn = !turn;
		
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
					std::cout << "Invalid coordinates.\n";
				}
			}
			catch (std::exception e) {
				std::cout << "Invalid coordinates.";
			}
			dialog = "\nLaunching rockets..";
			coolprinting(dialog);
			//Send coordinates
			if (send(s, coordinates, sizeof(coordinates), 0) < 0) {
				std::cout << "Player disconnected. " << std::endl;
				prevLevel->beginPlay();
			}
			//Go into recieving mode
			int recv_size = recv(s, hit, sizeof(hit), 0);
			//Update grid
			if (hit[0] == '1') {
				playerCopy.grid[r][c] = 'X';
				player2.grid[r][c] = 'X';
				std::cout << "\nHit!" << std::endl;
			}
			else {
				player2.grid[r][c] = 'O';
				std::cout << "\nMissed!" << std::endl;
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
			for (int i = 0; i < gridSize; i++) {
				for (int j = 0; j < gridSize; j++) {
					if (player1.grid[i][j] == 'S')
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
			std::cout << "\nCoordinates: " << coordinates[0] << coordinates[1] << coordinates[2] << std::endl;
			//Check if it hits and update grid
			
			r = coordinates[0] - '0' - 1;
			c = (int)coordinates[1] % 'a';

			if (coordinates[2] != NULL) {
				c = (int)coordinates[2] % 'a';
				r = ((coordinates[0] - '0') * 10 + (coordinates[1] - '0')) - 1;
			}
			
			int result = checkhit(player1.grid, r, c);

			if (result) {
				std::cout << "\nHit!" << std::endl;
				hit[0] = '1';
			}
			else {
				std::cout << "\nMissed!" << std::endl;
				hit[0] = '0';
			}

			//Send result
			if (send(s, hit, sizeof(hit), 0) < 0) {
				std::cout << "Player disconnected. " << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(900));
				prevLevel->beginPlay();
			}

			//check if game is going
			for (int i = 0; i < gridSize; i++) {
				for (int j = 0; j < gridSize; j++) {
					if (player1.grid[i][j] == 'S')
						alive1 = true;
				}
			}

			if (!alive1) {
				dialog = "\nThe enemies are victorious, comrade.";
				coolprinting(dialog);
				//send win
				win[0] = '1';
				if (send(s, win, sizeof(win), 0) < 0) {
					std::cout << "Player disconnected. " << std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(900));
					prevLevel->beginPlay();
				}
			}
			else {
				win[0] = '0';
				if (send(s, win, sizeof(win), 0) < 0) {
					std::cout << "Player disconnected. " << std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(900));
					prevLevel->beginPlay();
				}
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		system("cls");
		printGrids();
	} while (alive1 && alive2);
	closesocket(s);
	WSACleanup();
	prevLevel->beginPlay();
}