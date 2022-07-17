#pragma once
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <thread>
#include "Console.h"
#include "Player.h"
#pragma comment(lib, "winmm.lib") //Playsound library

class Level
{
	enum ships {
		Carrier = 5,
		Battleship = 4,
		Cruiser = 3,
		Submarine = 3,
		Destroyer = 2,
	};

	const int leftmargin = 3;
	const int topmargin = 1;
	const int dividor = 2;

public:
	Level* prevLevel;
	Player player1;
	Player player2;
	Player playerCopy;
	virtual void beginPlay() {};
	void printGrids();
	void initializeGrids();
	void coolprinting(std::string);
	int checkhit(char[gridSize][gridSize], int, int);
	void placeShips();
	void placeShips(ships, char[gridSize][gridSize]);

protected:
	std::string color = "\033[97m";
	std::string logo = R"(

~~~~~~~~~~~~~~~~~~~o~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                   o
                     o o     |   #)
                      oo    _|_|_#_  
                        o  |       |
   __             _________|       |________________
  |   -_____------                                   \
 >|    _____               Codeships          --->   )
  |__ -     ----_____________________________________/



)";
};

