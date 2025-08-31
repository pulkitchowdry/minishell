CC = cc
CFLAGS = -Wall -Wextra -Werror
LFLAGS = -lreadline

NAME = minishell

SRC = verification.c redirect_string_next_state.c tokenize.c parse.c parse_new_node.c process_initial_string.c wildcard.c built_in_helper.c built_in_cmd.c main.c execute.c built_in_export.c exit.c helper_ext.c helper_ext_2.c helper_ext_3.c execute_2.c execute_3.c execute_4.c execute_5.c execute_6.c execute_7.c execute_8.c execute_9.c print.c parse_new_node_ext.c parse_new_node_ext_2.c parse_2.c parse_3.c parse_4.c parse_5.c parse_6.c redirect_string_2.c tokenize_2.c wildcard_2.c wildcard_3.c
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
