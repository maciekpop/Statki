#include "board.h"

using namespace std;

BattleshipBoard::BattleshipBoard() {
	for (int i = 0; i < BRDSIZE*BRDSIZE; i++) {
		yourBoard[i] = ' ';
		yourAttacks[i] = ' ';
		undoYourBoard[i] = ' ';
		computerBoard[i] = ' ';

	}
	for (int i = 0; i < (BRDSIZE + 2)*(BRDSIZE + 2); i++) {
		helpBoard[i] = ' ';
		if (i < (BRDSIZE + 2) || i >= ((BRDSIZE + 2)*(BRDSIZE + 2) - (BRDSIZE + 2)) || i == (BRDSIZE + 2) || i % (BRDSIZE + 2) == 0 || i % (BRDSIZE + 2) == 11)
			computerAttacks[i] = '.';
		else 
			computerAttacks[i] = ' ';
	}
	userTurn = (rand() % 2);
	genPos = rand() % ((BRDSIZE + 2)*(BRDSIZE + 2));
}

void BattleshipBoard::displayStats() {

	system("cls");
	cout << "-------------------------------------------------------\n";
	cout << "    |    Twoja plansza    |     | Plansza przeciwnika |\n";
	cout << "-------------------------------------------------------\n";
	cout << "    | 0 1 2 3 4 5 6 f 8 9 |     | 0 1 2 3 4 5 6 f 8 9 |\n";
	for (int i = 0; i < BRDSIZE; i++) {
		cout.width(3);
		cout << char(i + 97) << " | ";
		for (int j = 0; j < BRDSIZE; j++)
			cout << yourBoard[i*BRDSIZE + j] << ' ';
		cout << "| ";

		cout.width(3);
		cout << char(i + 97) << " | ";
		for (int j = 0; j < BRDSIZE; j++)
			cout << computerBoard[i*BRDSIZE + j] << ' ';
		cout << "| " << endl;
	}

	cout << endl << endl;

	if (numberOfYourHits > numberOfComputerHits)
		cout << "----------------------Wygrana!----------------------\n\n";
	else
		cout << "--------------------Przegrana...--------------------\n\n";

	cout <<"Twoja skutecznosc atakow: " << (numberOfYourHits * 100) / numberOfYourAttacks <<"%\n";
	cout << "Skutecznosc atakow rywala: " << (numberOfComputerHits * 100) / numberOfComputerAttacks << "%\n";
}

void BattleshipBoard::gamePlay() {

   cout << "**************************************************\n"
		<< "*                                                *\n"
	    << "*                     STATKI                     *\n"
		<< "*                                                *\n"
		<< "**************************************************\n"
		<< "*                     Zasady                     *\n"
		<< "* Statki nie moga sie stykac bokami ani rogami.  *\n"
		<< "* Do dyspozycji gracz ma 1 4-masztowca, 2        *\n"
		<< "* 3-masztowce, 3 2-masztowce i 4 1-masztowce.    *\n"
		<< "* Ataki sa zadawane na przemian, atak danego     *\n"
		<< "* gracza trwa do momentu gdy nie trafi.          *\n"
		<< "* Wygrywa ten kto pierwszy wyeliminuje flote     *\n"
		<< "* przeciwnika.                                   *\n"
		<< "**************************************************\n";

	initComputerBoard();

	char opt;
	cout << "Czy chcesz automatycznie ustawic statki? (y/n): ";
	cin >> opt;

	while (opt != 'y' && opt != 'Y' && opt != 'n' && opt != 'N' && opt != 't' && opt != 'T') {
		cout << "Czy chcesz automatycznie ustawic statki? (y/n): ";
		cin >> opt;
	}



	if (opt == 'n' || opt == 'N') {
		initYourBoard(false);
	}
	else { 
		initYourBoard(true); 
		cout << "Ustawiono automatycznie.\n";
	}

	if (userTurn) cout << "Rozpoczynasz gre.";
	else cout << "Gre rozpoczyna komputer.";

	Sleep(3000);

	for (int i = 0; i < (BRDSIZE + 2)*(BRDSIZE + 2); i++) {

		if (i < (BRDSIZE + 2) || i >= ((BRDSIZE + 2)*(BRDSIZE + 2) - (BRDSIZE + 2)) || i == (BRDSIZE + 2) || i % (BRDSIZE + 2) == 0 || i % (BRDSIZE + 2) == 11) 
			helpBoard[i] = '.';
		else 
			helpBoard[i] = ' ';
	}

	while (numberOfComputerHits != SQUARES && numberOfYourHits != SQUARES)
		attack();

	displayStats();
}

void BattleshipBoard::sunken(bool horizontal, int size) {

	genPos = undoGenPos;

	int i = CHANGE(genPos) / 10;
	int j = CHANGE(genPos) % 10;

	if (horizontal)
		updateHelpBoard(i, j, size, true);
	else
		updateHelpBoard(j, i, size, false);

	genPos = rand() % ((BRDSIZE + 2)*(BRDSIZE + 2));
	while (computerAttacks[genPos] == '@' || helpBoard[genPos] == '.')
		genPos = rand() % ((BRDSIZE + 2)*(BRDSIZE + 2));
	undoGenPos = genPos;
	serie = 0;
}

void BattleshipBoard::aim(bool horizontal, int size) {

	if (!horizontal)
	{
		if (helpBoard[genPos - (BRDSIZE + 2)] == ' ')
			genPos -= (BRDSIZE + 2);

		else if (helpBoard[genPos + (BRDSIZE + 2)] == ' ')
			genPos += (BRDSIZE + 2);

		else if (computerAttacks[genPos + BRDSIZE + 2] == '@' && helpBoard[genPos + size * (BRDSIZE + 2)] == ' ')
			genPos += size * (BRDSIZE + 2);

		else if (computerAttacks[genPos - (BRDSIZE + 2)] == '@' && helpBoard[genPos - size * (BRDSIZE + 2)] == ' ')
			genPos -= size * (BRDSIZE + 2);

		else
		{
			sunken(horizontal, size);

			switch (size)
			{
			case 2:
				twoCounter++;
				break;
			case 3:
				threeCounter++;
				break;
			default:
				break;
			}
		}
	}
	else
	{
		if (helpBoard[genPos - 1] == ' ')
			genPos--;

		else if (helpBoard[genPos + 1] == ' ')
			genPos++;

		else if (computerAttacks[genPos + 1] == '@' && helpBoard[genPos + size] == ' ')
			genPos += size;

		else if (computerAttacks[genPos - 1] == '@' && helpBoard[genPos - size] == ' ')
			genPos -= size;

		else
		{
			sunken(horizontal, size);

			switch (size)
			{
			case 2:
				twoCounter++;
				break;
			case 3:
				threeCounter++;
				break;
			default:
				break;
			}
		}
	}
}

int BattleshipBoard::setPosition(char* coords) {
	
	int col = 0, row = 0;

	if (coords[0] == 'A' || coords[0] == 'a' || coords[1] == 'A' || coords[1] == 'a')
		row = 0;
	else if (coords[0] == 'B' || coords[0] == 'b' || coords[1] == 'B' || coords[1] == 'b')
		row = 1;
	else if (coords[0] == 'C' || coords[0] == 'c' || coords[1] == 'C' || coords[1] == 'c')
		row = 2;
	else if (coords[0] == 'D' || coords[0] == 'd' || coords[1] == 'D' || coords[1] == 'd')
		row = 3;
	else if (coords[0] == 'E' || coords[0] == 'e' || coords[1] == 'E' || coords[1] == 'e')
		row = 4;
	else if (coords[0] == 'F' || coords[0] == 'f' || coords[1] == 'F' || coords[1] == 'f')
		row = 5;
	else if (coords[0] == 'G' || coords[0] == 'g' || coords[1] == 'G' || coords[1] == 'g')
		row = 6;
	else if (coords[0] == 'H' || coords[0] == 'h' || coords[1] == 'H' || coords[1] == 'h')
		row = 7;
	else if (coords[0] == 'I' || coords[0] == 'i' || coords[1] == 'I' || coords[1] == 'i')
		row = 8;
	else if (coords[0] == 'J' || coords[0] == 'j' || coords[1] == 'J' || coords[1] == 'j')
		row = 9;

	if (coords[1] == '0' || coords[0] == '0')
		col = 0;
	else if (coords[1] == '1' || coords[0] == '1')
		col = 1;
	else if (coords[1] == '2' || coords[0] == '2')
		col = 2;
	else if (coords[1] == '3' || coords[0] == '3')
		col = 3;
	else if (coords[1] == '4' || coords[0] == '4')
		col = 4;
	else if (coords[1] == '5' || coords[0] == '5')
		col = 5;
	else if (coords[1] == '6' || coords[0] == '6')
		col = 6;
	else if (coords[1] == '7' || coords[0] == '7')
		col = 7;
	else if (coords[1] == '8' || coords[0] == '8')
		col = 8;
	else if (coords[1] == '9' || coords[0] == '9')
		col = 9;

	return (BRDSIZE*row + col);
}

void BattleshipBoard::attack() {

	char coordinates[5];
	int position;
	bool hit = true;
	
	if (userTurn) {
		display();
		cout << "Twoja kolej.\n";
		Sleep(1000);
		while (hit && numberOfYourHits < SQUARES) {
			cout << "Podaj pozycje do ataku (np. h5): ";
			cin >> coordinates;
			position = setPosition(coordinates);

			while (yourAttacks[position] == '@' || yourAttacks[position] == '.' || position >= (BRDSIZE*BRDSIZE) || position < 0) {
				if(yourAttacks[position] == '@' || yourAttacks[position] == '.')
					cout << "Pole jest juz ustrzelone, wybierz inne: ";
				if (position >= (BRDSIZE*BRDSIZE) || position < 0)
					cout << "Pole ze zlego zakresu, wybierz inne: ";
				cin >> coordinates;
				position = setPosition(coordinates);

			}

			if (computerBoard[position] == ' ' || computerBoard[position] == '.') {
				display();
				cout << "Atak na " << coordinates;
				computerBoard[position] = '.';
				yourAttacks[position] = '.';
				Sleep(1000);
				display();
				cout << "Pudlo. " << coordinates <<endl;
				Sleep(2000);
				hit = false;
				userTurn = false;
				numberOfYourAttacks++;


			}
			else {
				display();
				cout << "Atak na " << coordinates;
				computerBoard[position] = '@';
				yourAttacks[position] = '@';
				Sleep(1000);
				display();
				cout << "Trafiony. " << coordinates <<endl;
				Sleep(2000);
				hit = true;
				numberOfYourHits++;
				numberOfYourAttacks++;
			}
		}
	}

	else { 
		
		bool horizontal;
			
		display();
		cout << "Kolej komputera.";
		Sleep(1000);

		while (hit && numberOfComputerHits < SQUARES)
		{
			switch (serie)
			{
			case 0:
				while (computerAttacks[genPos] == '@' || helpBoard[genPos] == '.')
					genPos = rand() % ((BRDSIZE + 2)*(BRDSIZE + 2));
				undoGenPos = genPos;
				break;

			case 1:
				if (twoCounter < TWO || threeCounter < THREE || fourCounter < FOUR) {
					if (helpBoard[genPos + (BRDSIZE + 2)] == ' ')
						genPos += (BRDSIZE + 2);
					else if (helpBoard[genPos - (BRDSIZE + 2)] == ' ')
						genPos -= (BRDSIZE + 2);
					else if (helpBoard[genPos + 1] == ' ')
						genPos++;
					else if (helpBoard[genPos - 1] == ' ')
						genPos--;
					else
					{
						sunken(1, 1);
						singleCounter++;
					}
				}
				else
				{
					sunken(1, 1);
					singleCounter++;
				}
				break;

			case 2:
				if (computerAttacks[genPos + BRDSIZE + 2] == '@' || computerAttacks[genPos - (BRDSIZE + 2)] == '@')
					horizontal = false;
				else horizontal = true;

				if (threeCounter < THREE || fourCounter < FOUR) 
					aim(horizontal, 2);

				else
				{
					sunken(horizontal, 2);
					twoCounter++;
				}
				break;

			case 3:
				if (computerAttacks[genPos + BRDSIZE + 2] == '@' || computerAttacks[genPos - (BRDSIZE + 2)] == '@')
					horizontal = false;
				else horizontal = true;

				if (fourCounter < FOUR) 
					aim(horizontal, 3);
			
				else
				{
					sunken(horizontal, 3);
					threeCounter++;
				}
				break;

			case 4:
				if (computerAttacks[genPos + BRDSIZE + 2] == '@' || computerAttacks[genPos - (BRDSIZE + 2)] == '@')
					horizontal = false;
				else horizontal = true;

				if (!horizontal) 
				{
					sunken(horizontal, 4);
					fourCounter++;
				}

				else 
				{
					sunken(horizontal, 4);
					fourCounter++;
				}
				break;

			default:
				break;
			}

			if (yourBoard[CHANGE(genPos)] == ' ') {
				display();
				cout << "Atak na ";
				cout << char((CHANGE(genPos)/BRDSIZE) + 97) << (CHANGE(genPos)%BRDSIZE) <<endl;
				yourBoard[CHANGE(genPos)] = '.';
				computerAttacks[genPos] = '.';
				helpBoard[genPos] = '.';
				Sleep(1000);
				display();
				cout << "Pudlo. " << char((CHANGE(genPos) / BRDSIZE) + 97) << (CHANGE(genPos) % BRDSIZE) <<endl;
				Sleep(2000);
				hit = false;
				userTurn = true;
				numberOfComputerAttacks++;
				genPos = undoGenPos;
				
			}
			else {
				display();
				cout << "Atak na ";
				cout << char((CHANGE(genPos) / BRDSIZE) + 97) << (CHANGE(genPos) % BRDSIZE) << endl;
				yourBoard[CHANGE(genPos)] = '@';
				computerAttacks[genPos] = '@';
				helpBoard[genPos] = '@';
				Sleep(1000);
				display();
				cout << "Trafiony. " << char((CHANGE(genPos) / BRDSIZE) + 97) << (CHANGE(genPos) % BRDSIZE) <<endl;
				Sleep(2000);
				hit = true;
				numberOfComputerHits++;
				numberOfComputerAttacks++;
				serie++;
				if (genPos == undoGenPos - 1 || genPos == undoGenPos - (BRDSIZE + 2))
					undoGenPos = genPos;
			}
		}
	}
}


void BattleshipBoard::findPlaceForShip(char* b, int size) {

	bool collision = true;
	bool horizontal;
	int posX, posY;

	while (collision) {

		horizontal = (rand() % 2);
		posX = rand() % BRDSIZE;
		posY = rand() % (BRDSIZE - size + 1);

		if (horizontal)
		{
			for (int i = 0; i < size; i++) {
				if (helpBoard[(BRDSIZE + 2)*(posX + 1) + (posY + 1) + i] == '.') {
					collision = true;
					break;
				}
				else collision = false;
			}

			if (!collision) {
				for (int i = 0; i < size; i++)
					b[BRDSIZE * posX + posY + i] = 'O';
				updateHelpBoard(posX, posY, size, horizontal);
			}
		}

		else
		{
			for (int i = 0; i < size; i++) {
				if (helpBoard[(BRDSIZE + 2)*(posY + 1) + (posX + 1) + (BRDSIZE + 2)*i] == '.') {
					collision = true;
					break;
				}
				else collision = false;
			}

			if (!collision) {
				for (int i = 0; i < size; i++)
					b[BRDSIZE * posY + posX + BRDSIZE * i] = 'O';
				updateHelpBoard(posX, posY, size, horizontal);
			}
		}
	}
}

void BattleshipBoard::initComputerBoard() {

	int i;

	for (i = 0; i < FOUR; i++)
		findPlaceForShip(computerBoard, 4);
	
	for (i = 0; i < THREE; i++)
		findPlaceForShip(computerBoard, 3);
	
	for (i = 0; i < TWO; i++)
		findPlaceForShip(computerBoard, 2);
	
	for (i = 0; i < SINGLE; i++)
		findPlaceForShip(computerBoard, 1);

	clearHelpBoard();
}

void BattleshipBoard::clearHelpBoard() {
	for (int i = 0; i < (BRDSIZE + 2)*(BRDSIZE + 2); i++)
		helpBoard[i] = ' ';
}

void BattleshipBoard::updateYourUndoBoard() {
	for (int i = 0; i < BRDSIZE*BRDSIZE; i++)
		undoYourBoard[i] = yourBoard[i];
}

void BattleshipBoard::updateHelpBoard(int i, int j, int size, bool horizontal) {

	if (horizontal) {

		for (int k = 0; k < size; k++) {
			helpBoard[(BRDSIZE + 2) * (i + 0) + (j + 1) + k] = '.';
			helpBoard[(BRDSIZE + 2) * (i + 1) + (j + 1) + k] = '.';
			helpBoard[(BRDSIZE + 2) * (i + 2) + (j + 1) + k] = '.';
		}

		helpBoard[(BRDSIZE + 2) * (i + 0) + j] = '.';
		helpBoard[(BRDSIZE + 2) * (i + 1) + j] = '.';
		helpBoard[(BRDSIZE + 2) * (i + 2) + j] = '.';

		helpBoard[(BRDSIZE + 2) * (i + 0) + j + size + 1] = '.';
		helpBoard[(BRDSIZE + 2) * (i + 1) + j + size + 1] = '.';
		helpBoard[(BRDSIZE + 2) * (i + 2) + j + size + 1] = '.';
	}

	else {
		for (int k = 0; k < size; k++) {
			helpBoard[(BRDSIZE + 2) * (j + 1) + (i + 0) + (BRDSIZE + 2) * k] = '.';
			helpBoard[(BRDSIZE + 2) * (j + 1) + (i + 1) + (BRDSIZE + 2) * k] = '.';
			helpBoard[(BRDSIZE + 2) * (j + 1) + (i + 2) + (BRDSIZE + 2) * k] = '.';
		}

		helpBoard[(BRDSIZE + 2) * j + i + 0] = '.';
		helpBoard[(BRDSIZE + 2) * j + i + 1] = '.';
		helpBoard[(BRDSIZE + 2) * j + i + 2] = '.';

		helpBoard[(BRDSIZE + 2) * (j + size + 1) + i + 0] = '.';
		helpBoard[(BRDSIZE + 2) * (j + size + 1) + i + 1] = '.';
		helpBoard[(BRDSIZE + 2) * (j + size + 1) + i + 2] = '.';
	}
}

void BattleshipBoard::setShipPos(int decks) {

	updateYourUndoBoard();

	for (int i = 0; i < decks; i++)
		yourBoard[i] = 'O';

	bool horizontal = true;

	int bow = 0;

	display();
	cout << "Operuj strzalkami zeby ustawic statek.\nWcisnij rr zeby obrocic statek.\nWcisnij Spacje zeby zatwierdzic pozycje.\n";

	while (_getch() != ' ') {

		switch (_getch())
		{
		case KEY_UP:
			if (horizontal)
			{
				for (int i = bow; i < bow + decks; i++) {
					yourBoard[i - BRDSIZE] = 'O';
					if (undoYourBoard[i] == 'O')
						yourBoard[i] = 'O';
					else yourBoard[i] = ' ';
					if (bow == i) bow = i;
				}
			}
			else {
				for (int i = bow; i < bow + BRDSIZE * decks; i += BRDSIZE) {
					yourBoard[i - BRDSIZE] = 'O';
					if (undoYourBoard[i] == 'O')
						yourBoard[i] = 'O';
					else yourBoard[i] = ' ';
					if (bow == i) bow = i;
				}
			}
			display();
			cout << "Operuj strzalkami zeby ustawic statek.\nWcisnij rr zeby obrocic statek.\nWcisnij Spacje zeby zatwierdzic pozycje.\n";
			bow -= BRDSIZE;
			break;

		case KEY_DOWN:
			if (horizontal)
			{
				for (int i = bow; i < bow + decks; i++) {
					yourBoard[i + BRDSIZE] = 'O';
					if (undoYourBoard[i] == 'O')
						yourBoard[i] = 'O';
					else yourBoard[i] = ' ';
					if (bow == i) bow = i;
				}
			}
			else {
				for (int i = (bow + BRDSIZE * (decks - 1)); i >= bow; i -= BRDSIZE) {
					yourBoard[i + BRDSIZE] = 'O';
					if (undoYourBoard[i] == 'O')
						yourBoard[i] = 'O';
					else yourBoard[i] = ' ';
					if (bow == i) bow = i;
				}
			}
			display();
			cout << "Operuj strzalkami zeby ustawic statek.\nWcisnij rr zeby obrocic statek.\nWcisnij Spacje zeby zatwierdzic pozycje.\n";
			bow += BRDSIZE;
			break;

		case KEY_LEFT:
			if (horizontal)
			{
				for (int i = bow; i < bow + decks; i++) {
					yourBoard[i - 1] = 'O';
					if (undoYourBoard[i] == 'O')
						yourBoard[i] = 'O';
					else yourBoard[i] = ' ';
					if (bow == i) bow = i;
				}
			}
			else {
				for (int i = bow; i < bow + BRDSIZE * decks; i += BRDSIZE) {
					yourBoard[i - 1] = 'O';
					if (undoYourBoard[i] == 'O')
						yourBoard[i] = 'O';
					else yourBoard[i] = ' ';
					if (bow == i) bow = i;
				}
			}
			display();
			cout << "Operuj strzalkami zeby ustawic statek.\nWcisnij rr zeby obrocic statek.\nWcisnij Spacje zeby zatwierdzic pozycje.\n";
			bow--;
			break;

		case KEY_RIGHT:
			if (horizontal)
			{
				for (int i = (bow + decks - 1); i >= bow; i--) {
					yourBoard[i + 1] = 'O';
					if (undoYourBoard[i] == 'O')
						yourBoard[i] = 'O';
					else yourBoard[i] = ' ';
					if (bow == i) bow = i;
				}
			}
			else {
				for (int i = bow; i < bow + BRDSIZE * decks; i += BRDSIZE) {
					yourBoard[i + 1] = 'O';
					if (undoYourBoard[i] == 'O')
						yourBoard[i] = 'O';
					else yourBoard[i] = ' ';
					if (bow == i) bow = i;
				}
			}
			display();
			cout << "Operuj strzalkami zeby ustawic statek.\nWcisnij rr zeby obrocic statek.\nWcisnij Spacje zeby zatwierdzic pozycje.\n";
			bow++;
			break;

		case ROTATE:
			if (horizontal) {
				for (int i = 1; i < decks; i++) {
					yourBoard[bow + BRDSIZE * i] = 'O';
					if (undoYourBoard[bow + i] == 'O')
						yourBoard[bow + i] = 'O';
					else yourBoard[bow + i] = ' ';

				}
				horizontal = false;
			}
			else {
				for (int i = 1; i < decks; i++) {
					yourBoard[bow + i] = 'O';
					if (undoYourBoard[bow + BRDSIZE * i] == 'O')
						yourBoard[bow + BRDSIZE * i] = 'O';
					else yourBoard[bow + BRDSIZE * i] = ' ';

				}
				horizontal = true;
			}
			display();
			cout << "Operuj strzalkami zeby ustawic statek.\nWcisnij rr zeby obrocic statek.\nWcisnij Spacje zeby zatwierdzic pozycje.\n";
			break;

		default:
			break;
		}
	}
	display();
}

void BattleshipBoard::initYourBoard(bool autoset) {

	if (autoset) 
	{
		int i;

		for (i = 0; i < FOUR; i++)
			findPlaceForShip(yourBoard, 4);

		for (i = 0; i < THREE; i++)
			findPlaceForShip(yourBoard, 3);

		for (i = 0; i < TWO; i++)
			findPlaceForShip(yourBoard, 2);

		for (i = 0; i < SINGLE; i++)
			findPlaceForShip(yourBoard, 1);

		clearHelpBoard();
	}

	else 
	{
		int ship;
		int squareCounter = 0;
		int fourDecker = 0;
		int threeDecker = 0;
		int twoDecker = 0;
		int singleDecker = 0;


		while (squareCounter != SQUARES)
		{
			cout << "Podaj ilosc masztow: ";
			cin >> ship;
			switch (ship)
			{
			case 4:
				if (fourDecker != FOUR) {
					setShipPos(ship);
					squareCounter += 4;
					fourDecker++;
				}
				else cout << "Czteromasztowce gotowe\n";

				break;

			case 3:

				if (threeDecker != THREE) {
					setShipPos(ship);
					squareCounter += 3;
					threeDecker++;
				}
				else cout << "Trzymasztowce gotowe\n";

				break;

			case 2:

				if (twoDecker != TWO) {
					setShipPos(ship);
					squareCounter += 2;
					twoDecker++;
				}
				else cout << "Dwumasztowce gotowe\n";

				break;

			case 1:

				if (singleDecker != SINGLE) {
					setShipPos(ship);
					squareCounter++;
					singleDecker++;
				}
				else cout << "Jednomasztowce gotowe\n";

				break;

			default:
				break;
			}
		}
	}
}

void BattleshipBoard::display() {

	system("cls");
	cout << "-------------------------------------------------------\n";
	cout << "    |    Twoja plansza    |     |     Twoje  ataki    |\n";
	cout << "-------------------------------------------------------\n";
	cout << "    | 0 1 2 3 4 5 6 7 8 9 |     | 0 1 2 3 4 5 6 7 8 9 |\n";
	for (int i = 0; i < BRDSIZE; i++) {
		cout.width(3);
		cout << char(i + 97) << " | ";
		for (int j = 0; j < BRDSIZE; j++) 
			cout << yourBoard[i*BRDSIZE + j] << ' ';
		cout << "| ";

		cout.width(3);
		cout << char(i + 97) << " | ";
		for (int j = 0; j < BRDSIZE; j++)
			cout << yourAttacks[i*BRDSIZE + j] << ' ';
		cout << "| " << endl;
		
	}
	cout << endl;
}