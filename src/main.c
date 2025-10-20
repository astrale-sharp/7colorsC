/*
	Fun implemtation of a fun game: seven colors
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <termios.h>
#include "board.h"
#include "gamelogic.h"

#define ERRMSG "Provide a number between 3 and 200.\n"

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		printf(ERRMSG);
		return 0;
	}
	int	size = atoi(av[1]);
	if (!(size >= 3 && size <= 200))
	{
		printf(ERRMSG);
		return 0;
	}
	struct termios old, new;
	tcgetattr(0, &old);
	tcgetattr(0, &new);
	new.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &new);
	play(size);
	tcsetattr(0, TCSANOW, &old);
	return 0;
}
