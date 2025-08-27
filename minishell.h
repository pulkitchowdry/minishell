/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:32:25 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/27 19:02:39 by pchowdry         ###   ########.fr       */
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

typedef struct s_variable_context
{
	char	**environment_variables;
	char	**local_variables;
	char	**dup_environment_variables;
}	t_variable_context;

typedef struct s_state_context
{
	t_ast_node			*current;
	t_variable_context	*context;
}	t_state_context;

typedef struct s_parse_context
{
	t_ast_node	*root;
	t_list		*tokens;
}	t_parse_context;

typedef struct s_env
{
	char	*oldpwd; //For cd
	char	*pwd; //For cd
	char	**new_env; //For export to store both key and value
	char	*new_env_key; //For export
	char	*new_env_value; //For export
	char	*char_unset; //For unset
	int		no_new_line_flag; //For echo
}	t_env;

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
	char	**myenvp; //For export to duplicate envp
}	t_data;

char		**ft_split(char const *s, char c);
char		*ft_strjoin(char const *s1, char const *s2);
size_t		ft_strlen(const char *str);
t_list		*create_tokens(char *string);
void		free_token(void *token);

void		free_command(t_ast_node **current);
int			initialize_command_state(t_token *cur_tok, t_ast_node **current);
int			process_assign_string_val(t_list **list, t_token *cur_tok,
				t_ast_node **current, t_variable_context *context);
int			process_assign_space(t_list **list, t_ast_node **current);
int			process_assign_op(t_list **list, t_ast_node **current);
int			process_initial_command_string(t_list **list, t_token *cur_tok,
				t_ast_node **current, t_variable_context *context);
int			process_command_space(t_list **list, t_ast_node **current);
int			process_redirect(t_list **list, t_token *cur_tok,
				t_ast_node **current, int current_state);
int			process_command_string(t_list **list, t_token *cur_tok,
				t_ast_node **current, t_variable_context *context);
int			process_redirect_string(t_list **list, t_token *cur_tok,
				int current_state, t_state_context *state_context);
int			process_redirect_space(t_list **list,
				t_ast_node **current, int current_state);
t_ast_node	*extract_command(t_list **list, t_state_context *context);
t_ast_node	*parse_command(t_list **list, t_state_context *context);
t_ast_node	*parse_pipeline(t_list **list, t_state_context *context);
t_ast_node	*parse_list(t_list **list, t_state_context *context);

char		is_pipe_or_logical(int type);
char		is_command_string(int type);
void		append_string_to_current_node(t_token *cur_tok,
				t_list *list, t_variable_context *context);
int			process_state(int current_state, t_list **list,
				t_token *cur_tok, t_state_context *context);
t_ast_node	*parse_bracket(t_list **list, t_state_context *context);
t_ast_node	*free_and_return_zero(t_ast_node *current);
t_token		*advance_new_node(t_list **list);
char		is_extractable(int type);
char		is_redirect(int type);
char		is_pipe_or_logical(int type);
char		is_command_string(int type);
char		is_wildcard_present(char *string);
t_ast_node	*parse_new_node(t_state_context *state_context,
				t_token *cur_tok, t_list **list,
				t_ast_node *(*func)(t_list **, t_state_context *));
int			determine_next_redirect_string_if_redirect(int current_state);
int			determine_next_redirect_string_if_command(int current_state);
int			determine_next_redirect_string_if_space(int current_state);
int			process_initial_assignment(t_token *cur_tok, t_ast_node **current);
int			process_initial_else(t_token *cur_tok, t_ast_node **current);
int			process_initial_spaces(t_token *cur_tok, t_ast_node **current);
int			process_initial_redirection(t_token *cur_tok, t_ast_node **current);
int			process_initial_quotes_or_variable(t_token *cur_tok,
				t_ast_node **current, t_variable_context *context);
int			determine_next_redirect_string_if_space(int current_state);
int			determine_next_redirect_string_if_command(int current_state);
int			determine_next_redirect_string_if_redirect(int current_state);
int			determine_next_redirect_if_command_string(int current_state);
int			determine_next_redirect_if_space(int current_state);
int			determine_next_redirect_space_if_space(int current_state);
int			determine_next_redirect_space_if_string(int current_state);
void		process_additional_tokens(t_list **result, char **string);
char		*process_double_quote(char *string, t_variable_context *context);
char		*process_single_quote(char *string);
char		*process_variable(char *string, t_variable_context *context);
char		*process_wildcard(t_ast_node *current_node,
				char *string, t_list *list);
t_list		*find_match_string(char *string_with_wildcard);
t_list		*find_matches(char head_flag, char tail_flag,
				char **list_of_strings);
void		free_string_array(char **string_array);
size_t		ft_size(char **string_array);
char		is_safe_to_execute(t_ast_node *node);
int		execute_heredoc(t_ast_node *root, t_list *tokens, t_ast_node *node, int ret_code);
int		execute_command_ast(t_ast_node *root, t_list *tokens, t_ast_node *node, t_variable_context *context);
int			execute_and_wait_child(t_ast_node *node, t_variable_context *context);
void		unlink_files(t_ast_node *node);
void		print_signal(int);
char		*get_variable_key(char *string);
char		*get_variable_value(char *string);
char		**append_string_array(char **string_array, char *string);
size_t	ft_env_length(char **envp);
char	**ft_copy_envp(char **myenvp, char *new);
char	**ft_dup_str_array(char **envp);
char	**ft_dup_envp(char **envp);
void	ft_add_to_myenvp(t_variable_context *context, t_env *temp_env);
// char	**ft_add_to_local(t_env *temp_env, t_variable_context *context);
void	ft_update_envp(t_env *temp_envp, t_list *command, t_variable_context *context);
char	*ft_extract_envp(char **envp, char *str);
char	**ft_remove_from_dup_envp(t_variable_context *context, t_env *temp_envp);
char	**ft_remove_from_myenvp(t_variable_context *context, t_env *temp_envp);
char	**ft_remove_from_local(t_variable_context *context, t_env *temp_envp);
void	ft_echo(t_list *command, t_list *redirection, t_variable_context *context);
void	ft_cd(t_list *command, t_list *redirection, t_variable_context *context);
void	ft_pwd(t_list *command, t_list *redirection, t_variable_context *context);
void	ft_export(t_list *command, t_list *redirection, t_variable_context *context);
void	ft_unset(t_list *command, t_list *redirection, t_variable_context *context);
void	ft_env(t_list *command, t_list *redirection, t_variable_context *context);
void	ft_exit(t_list *command, t_list *redirection, t_variable_context *context);
int	append_local_variables(t_list *assignment, t_list *redirection, t_variable_context *context);

#endif
