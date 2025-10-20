#include "board.h"

int	**board_new(int size)
{
	int	**board = malloc(sizeof(int *) * size);
	for (int k = 0; k < size; k++)
	{
		board[k] = malloc(sizeof(int) * size);
		for (int j = 0; j < size; j++)
			board[k][j] = 0;
	}
	return board;
}

int	**board_cpy(int **src, int size)
{
	int	**board = malloc(sizeof(int *) * size);
	for (int k = 0; k < size; k++)
	{
		board[k] = malloc(sizeof(int) * size);
		for (int j = 0; j < size; j++)
			board[k][j] = src[k][j];
	}
	return board;
}

void	board_free(int **board, int size)
{
	for (int k = 0; k < size; k++)
	{
		free(board[k]);
	}
	free(board);
}

int	get_rand_tile()
{
	return rand() % 5 + 2;
}

void	board_random_populate(int **board, int size)
{
	for (int y = 0; y < size; y++)
		for (int x = 0; x < size; x++)
			board[x][y] = get_rand_tile();
}

char	*get_color_pstr(int c)
{
	if (c < 0 || c >= 7)
		return NULL;
	char *cols[] = {COLP1, COLP2, COLN1, COLN2, COLN3, COLN4, COLN5};
	return cols[c];
}

void	board_print_tile(int color)
{
	char *s = get_color_pstr(color);
	if (s == NULL)
	{
		printf("ERROR COLOR PROVIDED IS OUT OF RANGE IN TABLE: %d", color);
		exit(1);
	}
	char res[50];
	res[0] = 0;
	strcat(res, s);
	strcat(res, "  ");
	strcat(res, RESET);
	printf("%s", res);
}

void	board_print(int **board, int size)
{
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			board_print_tile(board[x][y]);
		}
		printf("\n");
	}
}

void	board_double_print(int **b1, int **b2, int size)
{
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			board_print_tile(b1[x][y]);
		}
		printf("\t");
		for (int x = 0; x < size; x++)
		{
			board_print_tile(b2[x][y]);
		}
		printf("\n");
	}
}

void	iter(int it[2], int n)
{
	if (n == 0)
	{
		it[0] = 0;
		it[1] = 1;
	}
	else if (n == 1)
	{
		it[0] = 0;
		it[1] = -1;
	}
	else if (n == 2)
	{
		it[0] = 1;
		it[1] = 0;
	}
	else if (n == 3)
	{
		it[0] = -1;
		it[1] = 0;
	}
}

bool	is_in_bounds(int pos, int size)
{
	return (pos >= 0 && pos < size);
}

void	board_get_opt(int **b, int size, int opti[5], bool p1_turn)
{
	int playercolor = !p1_turn;
	for (int i = 0; i < 5; i++)
		opti[i] = 0;
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			if (b[x][y] == playercolor)
			{
				for (int i = 0; i < 4; i++)
				{
					int it[2];
					iter(it, i);
					int nx = x + it[0];
					int ny = y + it[1];
					printf("CN(%d %d) ", nx , ny);
					if (!is_in_bounds(nx, size) || !is_in_bounds(ny, size))
						continue ;
					int nc = b[nx][ny]; 
					if (nc > 1)
						opti[nc - 2] = 1;
				}
			}
		}
	}
}

void	board_apply(int **b, int size, int color, bool p1_turn)
{
	bool found = 1;
	int playercolor = !p1_turn;
	while (found)
	{
		found = 0;
		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < size; y++)
			{
				if (!(b[x][y] == playercolor))
				{
					continue ;
				}
				for (int i = 0; i < 4; i++)
				{
					int it[2];
					iter(it, i);
					int nx = x + it[0];
					int ny = y + it[1];
					if (!is_in_bounds(nx, size) || !is_in_bounds(ny, size))
						continue ;
					int nc = b[nx][ny]; 
					if (nc == color)
					{
						found = 1;
						b[nx][ny] = !p1_turn;
					}
				}
			}
		}
	}
}

/* p1 counted positively, p2 negatively, if game not finished, ret 0 */
void	board_count_point(int **b, int size, int *p1, int *p2)
{
	(*p1) = 0;
	(*p2) = 0;
	for (int y = 0; y < size; y++)
		for (int x = 0; x < size; x++)
			if (b[x][y] == 0)
				(*p1)++;
			else if (b[x][y] == 1)
				(*p2)++;
}
