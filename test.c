
#include <termios.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
	struct termios termios_p;
	tcgetattr(0 , &termios_p);
	termios_p.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &termios_p);
	char r;
	while (1)
	{
		r = fgetc(stdin);
		printf("%c", r + 1);
		if (r == 'x')
			break ;
	}
	termios_p.c_lflag |= ICANON | ECHO;
	tcsetattr(0, TCSANOW, &termios_p);
	return 0;
}
