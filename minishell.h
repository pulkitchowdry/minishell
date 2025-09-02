/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:32:25 by pchowdry          #+#    #+#             */
/*   Updated: 2025/09/02 14:46:47 by pchowdry         ###   ########.fr       */
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
	char	*oldpwd;
	char	*pwd;
	char	*new_env_key;
	char	*new_env_value;
	char	*local_key;
	char	*local_value;
	char	*char_unset;
	int		no_new_line_flag;
}	t_env;

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
int			execute_heredoc(t_ast_node *root,
				t_list *tokens, t_state_context *state_context, int ret_code);
int			execute_command_ast(t_ast_node *root,
				t_list *tokens, t_ast_node *node, t_variable_context *context);
void		unlink_files(t_ast_node *node);
void		print_signal(int signal);
void		print_new_line(int signal);
char		*get_variable_key(char *string);
char		*get_variable_value(char *string);
char		**append_string_array(char **string_array, char *string);
size_t		ft_env_length(char **envp);
char		**ft_copy_envp(char **myenvp, char *new);
char		**ft_dup_str_array(char **envp);
char		**ft_dup_envp(char **envp);
void		ft_add_to_myenvp(t_variable_context *context, t_env *temp_env);
void		ft_update_envp(t_env *temp_envp,
				t_list *command, t_variable_context *context);
char		*ft_extract_envp(char **envp, char *str);
char		**ft_remove_from_dup_envp(t_variable_context *context,
				t_env *temp_envp);
char		**ft_remove_from_myenvp(t_variable_context *context,
				t_env *temp_envp);
char		**ft_remove_from_local(t_variable_context *context,
				t_env *temp_envp);
int			ft_echo(t_list *command);
int			ft_cd(t_list *command,
				t_variable_context *context);
int			ft_pwd(void);
int			ft_export(t_list *command, t_list *redirection,
				t_variable_context *context);
int			ft_unset(t_list *command,
				t_variable_context *context);
int			ft_env(t_variable_context *context);
int			ft_exit(t_ast_node *root,
				t_list *token, t_ast_node *node, t_variable_context *context);
int			append_local_variables(t_list *assignment,
				t_list *redirection, t_variable_context *context);
int			append_local_variables_2(t_list *assignment,
				t_list *redirection, t_variable_context *context);
char		*ft_get_key(char *str);
char		*ft_get_value(char *str);
int			ft_find_equal(char *str);
void		ft_add_to_others(char *str, t_variable_context *context);
char		*ft_oldpwd(char *str, size_t start);
int			fork_heredoc(t_ast_node *root, t_list *tokens, t_list *list,
				t_variable_context *context);
int			print_permission_denied_child(t_ast_node *root, t_list *tokens,
				char *string, t_variable_context *context);
void		compare_redirect_input_child(t_ast_node *root, t_list *tokens,
				t_list *redirection, t_variable_context *context);
void		configure_redirection_child(t_ast_node *root, t_list *tokens,
				t_list *redirection,
				t_variable_context *context);
void		execute_with_execve(t_list *command, char **envp);
char		**extract_path_variable(char **environment_variable);
int			open_files(t_list *redir);
char		print_permission_denied_root(char *string);
void		dup_fd_to_stdout(int fd);
void		save_stdio(void);
char		configure_redirection_root(t_list *redirection);
t_list		*expand_command_wildcard_and_quotes(t_list *command,
				t_variable_context *context);
char		is_builtin_command(char *command);
int			fork_and_wait(t_ast_node *root,
				t_list *tokens, t_ast_node *node,
				t_variable_context *context);
int			open_temp_files(t_ast_node *root, t_list *tokens, t_list *list,
				t_variable_context *context);
void		exec_pipe_left_child(t_parse_context *parse_context,
				int *fd, t_ast_node *node, t_variable_context *context);
void		exec_pipe_right_child(t_parse_context *parse_context,
				int *fd, t_ast_node *node, t_variable_context *context);
void		close_all_pipes(int *fd);
int			execute_command(t_ast_node *root,
				t_list *token, t_ast_node *node,
				t_variable_context *context);
int			execute_logical_or(t_ast_node *root,
				t_list *tokens,
				t_ast_node *node,
				t_variable_context *context);
int			execute_logical_and(t_ast_node *root,
				t_list *tokens,
				t_ast_node *node,
				t_variable_context *context);
int			ft_add_to_myenvp_2(t_env *temp_env, char *temp_key,
				t_variable_context *context);
char		*join_dup_envp(char *key, char *value);
char		*get_special_variable(char *variable);
char		*extract_variable_name(char *string, char *start, char *end);
char		*substitute_variable(char *string, char *start,
				char *end, t_variable_context *context);
t_list		*construct_single_quote(char **string);
t_list		*construct_double_quote(char **string);
t_list		*construct_1_character(char **string);
t_list		*construct_2_characters(char **string);
t_list		*construct_string(char **string);
void		sort_string_ascii(char **string);
char		test_for_both_ends(char **list_of_strings, char *filename);
char		test_for_tail(char **list_of_strings, char *filename);
char		test_for_head(char **list_of_strings, char *filename);
void		compact_filenames(int max_files, char **filenames);
char		**get_sorted_filenames(int dir_num);
size_t		min(size_t a, size_t b);
void		print_ast(t_ast_node *root);
void		substitute_node_for_variable(t_list **result, t_list **prev,
				t_list **command, t_variable_context *context);
void		substitute_node_for_single_quote(t_list **prev,
				t_list **command);
void		substitute_node_for_double_quote(t_list **prev,
				t_list **command, t_variable_context *context);
void		substitute_node_with_list(t_list **result,
				t_list **prev, t_list **command);
int			ft_cd_path_len(char *str);
char		*ft_cd_path(char *str);
char		*ft_get_cd_path(t_list *command);
int			ft_cd_error(t_list *command);
void		ft_cd_home(t_env *temp_envp, t_list *command,
				t_variable_context *context);
int			ft_export_2_error(char *str);
void		ft_echo_new_line(t_env *temp_envp);
int			ft_asgmt(char *str, int type);

#endif
