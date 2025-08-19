/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:32:25 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/19 17:15:09 by chikoh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>

# include "libft/libft.h"

enum e_token_type
{
	DOUBLE_QUOTE,
	DOUBLE_QUOTE_STRING,
	SINGLE_QUOTE,
	SINGLE_QUOTE_STRING,
	COMMAND,
	HERE_DOC,
	REDIRECT_INPUT,
	REDIRECT_OUTPUT,
	REDIRECT_APPEND,
	PIPE,
	LOGICAL_AND,
	LOGICAL_OR,
	VARIABLE,
	WILDCARD,
	SPACES,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	ARGUMENTS,
	ASSIGNMENT,
	STRING
};

enum e_command_state
{
	ASSIGN_STRING_VAL,
	ASSIGN_SPACE,
	ASSIGN_OP,
	ASSIGN_REDIRECT,
	ASSIGN_REDIRECT_SPACE,
	ASSIGN_REDIRECT_STRING,
	INITIAL_COMMAND_STRING,
	COMMAND_SPACE,
	COMMAND_REDIRECT,
	COMMAND_REDIRECT_SPACE,
	COMMAND_REDIRECT_STRING,
	COMMAND_QUOTES,
	COMMAND_STRING,
	EXIT
};

typedef struct s_token
{
	enum e_token_type	type;
	char				*string;
}	t_token;

typedef struct s_ast_node
{
	t_token				*node;
	t_list				*command;
	t_list				*redirection;
	t_list				*assignment;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	t_ast_node;

typedef struct s_data
{
	char	*input;
	char	**cmd_dir;
	char	**cmd;
	char	*path;
	char	**path_dir;
	char	*p_temp;
	char	*cmd_path;
	pid_t	c_id;
	int		pipes;
	int		pipe_status;
	int		pipe_fd[2];
	int		prevfd;
}	t_data;

char		**ft_split(char const *s, char c);
char		*ft_strjoin(char const *s1, char const *s2);
size_t		ft_strlen(const char *str);
t_list		*create_tokens(char *string);
void		free_token(void *token);

void		free_command(t_ast_node **current);
int			initialize_command_state(t_token *cur_tok, t_ast_node **current);
int			process_assign_string_val(t_list **list,
				t_token *cur_tok, t_ast_node **current);
int			process_assign_space(t_list **list, t_ast_node **current);
int			process_assign_op(t_list **list, t_ast_node **current);
int			process_initial_command_string(t_list **list,
				t_token *cur_tok, t_ast_node **current);
int			process_command_space(t_list **list, t_ast_node **current);
int			process_redirect(t_list **list, t_token *cur_tok,
				t_ast_node **current, int current_state);
int			process_command_string(t_list **list, t_token *cur_tok,
				t_ast_node **current);
int			process_redirect_string(t_list **list, t_token *cur_tok,
				t_ast_node **current, int current_state);
int			process_redirect_space(t_list **list,
				t_ast_node **current, int current_state);
t_ast_node	*extract_command(t_list **list);
t_ast_node	*parse_command(t_list **list);
t_ast_node	*parse_pipeline(t_list **list);
t_ast_node	*parse_list(t_list **list);

char		is_pipe_or_logical(int type);
char		is_command_string(int type);
void		append_string_to_current_node(t_ast_node *current,
				t_token *cur_tok, t_list *list);
int			process_state(int current_state, t_list **list,
				t_token *cur_tok, t_ast_node **current);
t_ast_node	*parse_bracket(t_list **list);
t_ast_node	*free_and_return_zero(t_ast_node *current);
t_token		*advance_new_node(t_list **list);
char		is_extractable(int type);
char		is_redirect(int type);
char		is_pipe_or_logical(int type);
char		is_command_string(int type);
char		is_wildcard_present(char *string);
t_ast_node	*parse_new_node(t_ast_node *left_node,
				t_token *cur_tok, t_list **list,
				t_ast_node *(*func)(t_list **));
t_ast_node	*parse_command(t_list **list);
int			determine_next_redirect_string_if_redirect(int current_state);
int			determine_next_redirect_string_if_command(int current_state);
int			determine_next_redirect_string_if_space(int current_state);
int			process_initial_assignment(t_token *cur_tok, t_ast_node **current);
int			process_initial_else(t_token *cur_tok, t_ast_node **current);
int			process_initial_spaces(t_token *cur_tok, t_ast_node **current);
int			process_initial_redirection(t_token *cur_tok, t_ast_node **current);
int			process_initial_quotes_or_variable(t_token *cur_tok,
				t_ast_node **current);
int			determine_next_redirect_string_if_space(int current_state);
int			determine_next_redirect_string_if_command(int current_state);
int			determine_next_redirect_string_if_redirect(int current_state);
int			determine_next_redirect_if_command_string(int current_state);
int			determine_next_redirect_if_space(int current_state);
int			determine_next_redirect_space_if_space(int current_state);
int			determine_next_redirect_space_if_string(int current_state);
void		process_additional_tokens(t_list **result, char **string);
char		*process_double_quote(char *string);
char		*process_single_quote(char *string);
char		*process_variable(char *string);
char		*process_wildcard(t_ast_node *current_node,
				char *string, t_list *list);
t_list		*find_match_string(char *string_with_wildcard);
t_list		*find_matches(char head_flag, char tail_flag,
				char **list_of_strings);
void		free_string_array(char **string_array);
size_t		ft_size(char **string_array);
char		is_safe_to_execute(t_ast_node *node);
void		execute_heredoc(t_ast_node *node);
void		execute_command_ast(t_ast_node *node);
void		unlink_files(t_ast_node *node);
#endif
