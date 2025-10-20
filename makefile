NAME = sevencolors
CFLAGS = -Wall -Wextra -Werror -Wpedantic -Iinclude
SRC = $(addprefix src/, main.c board.c gamelogic.c)
OBJ = $(SRC:.c=.o)
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm $(OBJ)

fclean: clean
	-rm $(NAME)

re: fclean all
