#include "board.h"
#include <ctime>

int main()
{
	srand(time(NULL));

	BattleshipBoard B;
	B.gamePlay();
}