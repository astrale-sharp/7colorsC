#include "gamelogic.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int **init_board(int size)
{
	int	**board = board_new(size);
	board_random_populate(board, size);
	board[0][0] = 0;
	board[size - 1][size - 1] = 1;
	return board;
}

void make_optstr(int opti[5], char *optstr)
{
	for (int i = 0; i < 5; i++)
	{
		if (!opti[i])
			continue ;
		strcat(optstr, " ");
		strcat(optstr, get_color_pstr(i + 2));
		char res[2];
		res[1] = 0;
		res[0] = i + '0';
		strcat(optstr, res);
		strcat(optstr, RESET);
	}
}

void	move_cursor(int x, int y)
{
	printf("\033[%d;%dH", y, x);
}

void player_turn(int **b, int size, bool p1_turn)
{
	int	opti[5]; // idx at 0 means not an option, 1 , means an option
	board_get_opt(b, size, opti, p1_turn);
	char optstr[200] = "\nYour options are:";
	make_optstr(opti, optstr);
	int r = -1;
	do
	{
		move_cursor(0, 0);
		printf(CLEAR);
		if (r != -1)
			printf("You entered: `%c` You must enter one of the options.\n", (char)r);
		printf("%s\n", optstr);
		board_print(b, size);
		r = fgetc(stdin);
	} while (!(r >= '0' && r <= '4' && opti[r - '0']));
	int **cpy = board_cpy(b, size);
	board_apply(b, size, r - '0' + 2, p1_turn);
	move_cursor(0, 0);
	printf(CLEAR);
	printf("\nBoard updated. Press a touch to continue.\n");
	board_double_print(cpy, b, size);
	board_free(cpy, size);
	fgetc(stdin);
}

void	ai_turn(int **b, int size, bool p1_turn)
{
	int	opti[5]; // idx at 0 means not an option, 1 , means an option
	board_get_opt(b, size, opti, p1_turn);
	int choice = 0;
	while (!opti[choice] && choice < 5)
		choice++;
	int **cpy = board_cpy(b, size);
	board_apply(b, size, choice + 2, p1_turn);
	move_cursor(0, 0);
	printf(CLEAR);
	printf("\nAI Played.     Press a touch to continue.\n");
	board_double_print(cpy, b, size);
	board_free(cpy, size);
	fgetc(stdin);
}

void play(int size)
{
	int **b = init_board(size);
	bool p1_turn = true;
	int p1_pt = 0;
	int p2_pt = 0;
	while (!(p1_pt >= size * size / 2 || p2_pt >= size * size / 2))
	{
		if (p1_turn)
			player_turn(b, size, p1_turn);
		else
			ai_turn(b, size, p1_turn);
		p1_turn = !p1_turn;
		board_count_point(b, size, &p1_pt, &p2_pt);
	}
	move_cursor(0, 0);
	printf(CLEAR);		
	if (p1_pt >= size * size / 2)
		printf("Congratulations! You won with %d/%d tiles.\n", p1_pt, size * size);
	else
		printf("Damn! You lost with %d/%d tiles.\n", p1_pt, size * size);
	printf("Press any touch to exit the game.\n");
	board_print(b, size);
	board_free(b, size);
	fgetc(stdin);
}
