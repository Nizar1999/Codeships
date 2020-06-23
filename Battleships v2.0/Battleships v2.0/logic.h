#include "stdafx.h"
#pragma once
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

int const gridsize = 10;
enum ships {
	Carrier = 5,
	Battleship = 4,
	Cruiser = 3,
	Submarine = 3,
	Destroyer = 2,
};

//Game sequence
void intro();
void menu();
void menu2();
void multiplayerMenu();

//Game logic
void initializegrid();
void place_ships();
void place_ships(ships, char[gridsize][gridsize]);
int checkhit(char[gridsize][gridsize], int, int);

void offlineGameplay();
void multiplayerGameplay(bool);

//Utility functions
int count(int);
void MoveCursor(SHORT, SHORT);
void coolprinting(string);
void printgrids();
int initConnection(string, SOCKET&);
int initServer(SOCKET&);

#endif
