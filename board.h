#ifndef  BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
#include <conio.h>
#include <fstream>
#include <windows.h>
#include <iomanip>

#define BRDSIZE 10
#define FOUR 1
#define THREE 2
#define TWO 3
#define SINGLE 4
#define SQUARES (4*FOUR+3*THREE+2*TWO+SINGLE)
#define CHANGE(pos) (pos-((BRDSIZE+3)+(pos/(BRDSIZE+2))+((pos/(BRDSIZE+2))-2)))

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define ROTATE 114

using namespace std;

class BattleshipBoard {

	char computerBoard[BRDSIZE*BRDSIZE];
	char computerAttacks[(BRDSIZE + 2)*(BRDSIZE + 2)];

	int serie = 0;
	int fourCounter = 0;
	int threeCounter = 0;
	int twoCounter = 0;
	int singleCounter = 0;
	int genPos;
	int undoGenPos = 0;
	int numberOfComputerHits = 0;
	int numberOfComputerAttacks = 0;

	char yourBoard[BRDSIZE*BRDSIZE];
	char undoYourBoard[BRDSIZE*BRDSIZE];
	char yourAttacks[BRDSIZE*BRDSIZE];
	int numberOfYourHits = 0;
	int numberOfYourAttacks = 0;

	char helpBoard[(BRDSIZE + 2)*(BRDSIZE + 2)];
	bool userTurn;

	void displayStats();
	void sunken(bool horizontal, int size);
	void aim(bool horizontal, int size);
	void attack();
	void findPlaceForShip(char* b, int size);
	void updateYourUndoBoard();
	void updateHelpBoard(int i, int j, int size, bool horizontal);
	void clearHelpBoard();
	void initComputerBoard();
	void initYourBoard(bool autoset);
	void setShipPos(int decks);
	void display();
	int setPosition(char* coords);

public:

	BattleshipBoard();
	void gamePlay();

};
#endif