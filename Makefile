CC = cc
CFLAGS = -Wall -Wextra -Werror
LFLAGS = -lreadline

NAME = minishell

SRC = main.c built_in.c
HDR = minishell.h
OBJ = $(SRC:.c=.o)

LIBFT_DIR = libft
LIBFT_AR = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT_AR)
		$(CC) $(CFLAGS) $(OBJ) $(LIBFT_AR) $(LFLAGS) -o $(NAME)

$(LIBFT_AR):
		$(MAKE) -C $(LIBFT_DIR)

%.o: %.c $(HDR)
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -f $(OBJ)
		$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
		rm -f $(NAME)
		$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
