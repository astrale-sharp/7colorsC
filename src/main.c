/*
	Fun implemtation of a fun game: seven colors
*/

#include <termios.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "board.h"
#include "gamelogic.h"

#define USAGEMSG \
"Usage:\n\
sevencolors size.\n\
sevencolors \"serve\" size port.\n\
sevencolors \"connect\" address port.\n\
	size must be between 3 and 200.\n\
"
int	board_send(int fd, void *buf, int buflen)
{
	int idx = 0;
	while (idx != buflen)
	{
		int diff = send(fd, buf + idx, buflen - idx, 0);
		if (diff <= 0)
			return -1;
		idx += diff;
	}
	return 0;
}

int	board_recv(int fd, void *buf, int buflen)
{
	int idx = 0;
	while (idx != buflen)
	{
		int diff = recv(fd, buf + idx, buflen - idx, 0);
		if (diff <= 0)
			return -1;
		idx += diff;
	}
	return 0;
}



void 	move_cursor(int a, int b);
int remote_other_turn(int sfd, int **b, int size, int is_p1)
{
	move_cursor(0, 0);
	printf(CLEAR);
	printf("Other playing...\n");
	board_print(b, size);
	int choice;
	int status = board_recv(sfd, &choice, sizeof(int));
	if (status <= 0)
		return 1;
	int **cpy = board_cpy(b, size);
	board_apply(b, size, choice, !is_p1);
	move_cursor(0, 0);
	printf(CLEAR);
	printf("\nOther played.  Press a touch to continue.\n");
	board_double_print(cpy, b, size);
	board_free(cpy, size);
	fgetc(stdin);
	return 0;
}

int remote_player_turn(int sfd, int **b, int size, int is_p1)
{
	int	opti[5]; // idx at 0 means not an option, 1 , means an option
	board_get_opt(b, size, opti, is_p1);
	char optstr[200];
	sprintf(optstr, "You are:%s %s\nYour options are:", get_color_pstr(!is_p1), RESET);
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
	int choice = r - '0' + 2;
	board_apply(b, size, choice, is_p1);
	printf("Sending data...\n");
	int sent = board_send(sfd, &choice, sizeof(int));
	if (sent == -1)
		return 1;
	move_cursor(0, 0);
	printf(CLEAR);
	printf("\nBoard updated. Press a touch to continue.\n");
	board_double_print(cpy, b, size);
	board_free(cpy, size);
	fgetc(stdin);
	return 0;
}

void remote_play(int sfd, int is_p1, int **b, int size)
{
	printf("Starting remote play!\n");
	bool p1_turn = true;
	int p1_pt = 0;
	int p2_pt = 0;
	int status;
	while (!(p1_pt >= size * size / 2 || p2_pt >= size * size / 2))
	{
		if ((p1_turn && is_p1) || (!p1_turn && !is_p1))
			status = remote_player_turn(sfd, b, size, is_p1);
		else
			status = remote_other_turn(sfd, b, size, is_p1);
		if (status != 0)
		{
			printf("Connection error, exiting.\n");
			return ;
		}
		p1_turn = !p1_turn;
		board_count_point(b, size, &p1_pt, &p2_pt);
	}
	move_cursor(0, 0);
	printf(CLEAR);		
	if ((is_p1 && p1_pt >= size * size / 2) || (!is_p1 && p2_pt >= size * size / 2))
		printf("Congratulations! You won with %d/%d tiles.\n", (p1_pt * !is_p1) + (p2_pt * is_p1), size * size);
	else
		printf("Damn! You lost with %d/%d tiles.\n", (p1_pt * !is_p1) + (p2_pt * is_p1), size * size);
	printf("Press any touch to exit the game.\n");
	board_print(b, size);
	board_free(b, size);
	fgetc(stdin);
}

void	serveplay(int sfd, int size)
{
	if (!(size >= 3 && size <= 200))
	{
		printf("size must be between 3 and 200.\n");
		printf(USAGEMSG);
		return ;
	}
	printf("Sending size...\n");
	int status = send(sfd, &size, sizeof(int), 0);
	if (status == -1)
	{
		printf("Connection error exiting...");
		return ;
	}
	int is_p1 = rand() % 2 == 0;
	int notp1 = !is_p1;
	printf("Sending is_p1...\n");
	status = board_send(sfd, &notp1, sizeof(int));
	if (status == -1)
	{
		printf("Connection error exiting...");
		return ;
	}

	printf("Sending board...");
	int **board = init_board(size);
	for (int k = 0; k < size; k++)
	{
		status = board_send(sfd, board[k], sizeof(int) * size);
		if (status == -1)
		{
			printf("Connection error exiting...");
			board_free(board, size);
			return ;
		}
	}
	printf(" Sent!\n");

	struct termios old, new;
	tcgetattr(0, &old);
	tcgetattr(0, &new);
	new.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &new);
	remote_play( sfd, is_p1, board, size);
	tcsetattr(0, TCSANOW, &old);
}

void connectplay(int sfd)
{
	int size;
	int status = recv(sfd, &size, sizeof(int), 0);
	if (status == -1)
	{
		printf("Connection error exiting...");
		return ;
	}
	int is_p1;
	status = board_recv(sfd, &is_p1, sizeof(int));
	if (status == -1)
	{
		printf("Connection error exiting...");
		return ;
	}

	int	**board = malloc(sizeof(int*) * size);
	for (int k = 0; k < size; k++)
	{
		board[k] = malloc(sizeof (int) * size);
		status = board_recv(sfd, board[k], sizeof(int) * size);
		if (status == -1)
		{
			printf("Connection error exiting...");
			while (k >= 0)
			{
				free(board[k]);
				k--;
			}
			free(board);
			return ;
		}
	}
	struct termios old, new;
	tcgetattr(0, &old);
	tcgetattr(0, &new);
	new.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &new);
	remote_play(sfd, is_p1, board, size);
	tcsetattr(0, TCSANOW, &old);
}

void playsolo(char **av)
{
	int size = atoi(av[0]);
	if (!(size >= 3 && size <= 200))
	{
		printf("size must be between 3 and 200.\n");
		printf(USAGEMSG);
		return ;
	}
	struct termios old, new;
	tcgetattr(0, &old);
	tcgetattr(0, &new);
	new.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &new);
	play(size);
	tcsetattr(0, TCSANOW, &old);
}

void	servelogic(char **av)
{
	int                      sfd;
	struct addrinfo          hints;
	struct addrinfo          *result, *rp;
	char *port = av[1];
	int	size = atoi(av[0]);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
	hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
	hints.ai_protocol = 0;          /* Any protocol */
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	int status = getaddrinfo(NULL, port, &hints, &result);
	if (status != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}
	printf("Got addrinfo\n");
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue ;
		if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
			break; 
		close(sfd);
	}
	if (rp == NULL) {
		fprintf(stderr, "Could not bind\n");
		return;
	}
	printf("bound to sfd\n");
	printf("listening...");
	status = listen(sfd, 1);
	if (status == -1)
	{
		fprintf(stderr, "Could not listen\n");
		return;
	}
	printf(" OK\n");
	int goodfd = accept(sfd, rp->ai_addr, &rp->ai_addrlen);
	if (goodfd == -1)
	{
		fprintf(stderr, "Could not listen\n");
		return;
	}
	printf(" Accepted!\n");
	close(sfd);
	serveplay(goodfd, size);
	close(sfd);
	freeaddrinfo(result);
}


void	connectlogic(char **av)
{
		int                      sfd;
	struct addrinfo          hints;
	struct addrinfo          *result, *rp;
	char *addr = av[0];
	char *port = av[1];

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
	hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
	hints.ai_protocol = 0;          /* Any protocol */
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	int status = getaddrinfo(addr, port, &hints, &result);
	if (status != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue ;
		if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
			break; 
		close(sfd);
	}

	freeaddrinfo(result);
	if (rp == NULL) {
		fprintf(stderr, "Could not bind\n");
		exit(EXIT_FAILURE);
	}
	connectplay(sfd);
	close(sfd);
}


int	main(int ac, char **av)
{
	if (ac == 0)
	{
		printf("Not enough arguments!!\n");
		printf(USAGEMSG);
		return 0;
	}
	ac--;
	av++; // getting rid of program name
	srand(time(0));

	if (ac == 1)
		playsolo(av);
	else if (ac == 3)
	{
		if (strcmp(av[0], "serve") == 0)
			servelogic(av + 1);
		else if (strcmp(av[0], "connect") == 0)
			connectlogic(av + 1);
		else {
			printf("Expected 'connect' or 'serve' found %s\n", av[0]);
			printf(USAGEMSG);
			return 0;
		}
	}
	else {
		printf("Wrong argument count!!\n");
		printf(USAGEMSG);
		return 1;
	}
	return 0;
}
