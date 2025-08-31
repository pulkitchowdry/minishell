CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LFLAGS = -lreadline

NAME = minishell
BONUS_NAME = minishell_bonus

SRC = verification.c redirect_string_next_state.c tokenize.c parse.c parse_new_node.c process_initial_string.c wildcard.c built_in_helper.c built_in_cmd.c main.c execute.c built_in_export.c exit.c helper_ext.c helper_ext_2.c helper_ext_3.c execute_2.c execute_3.c execute_4.c execute_5.c execute_6.c execute_7.c execute_8.c execute_9.c execute_10.c print.c parse_new_node_ext.c parse_new_node_ext_2.c parse_2.c parse_3.c parse_4.c parse_5.c parse_6.c redirect_string_2.c tokenize_2.c wildcard_2.c wildcard_3.c
HDR = minishell.h
OBJ = $(SRC:.c=.o)

BONUS_SRC = $(SRC:.c=_bonus.c)
BONUS_HDR = minishell_bonus.h
BONUS_OBJ = $(BONUS_SRC:.c=.o)

LIBFT_DIR = libft
LIBFT_AR = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT_AR)
		$(CC) $(CFLAGS) $(OBJ) $(LIBFT_AR) $(LFLAGS) -o $(NAME)

$(LIBFT_AR):
		$(MAKE) -C $(LIBFT_DIR)

%.o: %.c $(HDR) $(BONUS_HDR)
		$(CC) $(CFLAGS) -c $< -o $@

bonus:	$(BONUS_OBJ) $(LIBFT_AR)
		$(CC) $(CFLAGS) $(BONUS_OBJ) $(LIBFT_AR) $(LFLAGS) -o $(BONUS_NAME)

clean:
		rm -f $(OBJ) $(BONUS_OBJ)
		$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
		rm -f $(NAME) $(BONUS_NAME)
		$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
