NAME = sevencolors
CFLAGS = -Wall -Wextra -Werror -Iinclude
SRC = $(addprefix src/, main.c board.c gamelogic.c)
OBJ = $(SRC:.c=.o)

all: $(NAME)

sanitize: CFLAGS_ADD =  -g3 -fsanitize=address
sanitize: all

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(CFLAGS_ADD) -o $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_ADD) -c $< -o $@

clean:
	-rm $(OBJ)

fclean: clean
	-rm $(NAME)

re: fclean all
