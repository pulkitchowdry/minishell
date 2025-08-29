/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 20:40:39 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/29 22:47:15 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

extern int	g_ret_code;

void	close_all_fds_in_heredoc(void)
{
	close(1023);
	close(0);
	close(1);
	close(2);
}

void	close_heredoc_pipes(int sig)
{
	(void)sig;
	close_all_fds_in_heredoc();
	exit(2);
}

void	write_temp_files(char *delimiter_string, int *fd)
{
	char	*line_string;
	char	delimiter[1000];

	ft_strlcpy(delimiter, delimiter_string, 1000);
	free(delimiter_string);
	dup2(fd[1], 1023);
	close(fd[1]);
	ft_putstr_fd("> ", 1);
	line_string = get_next_line(0);
	while (line_string != 0 && ft_strncmp(line_string, delimiter,
			ft_strlen(delimiter) + 1) != 0)
	{
		ft_putstr_fd(line_string, 1023);
		free(line_string);
		ft_putstr_fd("> ", 1);
		line_string = get_next_line(0);
	}
	if ((line_string == 0 || ft_strncmp(line_string, "", 1) == 0)
		&& ft_strncmp(delimiter, "\n", 2) != 0)
		ft_putstr_fd("here-document delimited by end-of-file", 2);
	free(line_string);
	close_all_fds_in_heredoc();
	exit(0);
}

int	wait_for_heredoc_to_finish(t_list *redirection_delimiter, int *fd, int pid)
{
	int	ret_code;

	signal(SIGINT, SIG_IGN);
	close(fd[1]);
	waitpid(pid, &ret_code, 0);
	signal(SIGINT, print_signal);
	if (WIFEXITED(ret_code) && WEXITSTATUS(ret_code) == 2)
	{
		close(fd[0]);
		return (130);
	}
	else if (WIFEXITED(ret_code))
	{
		free(redirection_delimiter->content);
		redirection_delimiter->content = ft_itoa(fd[0]);
		return (WEXITSTATUS(ret_code));
	}
	return (255);
}

int	fork_heredoc(t_ast_node *root, t_list *tokens, t_list *list,
					t_variable_context *context)
{
	int		ret_code;
	int		fd[2];
	int		pid;
	char	*line;

	ret_code = 0;
	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, close_heredoc_pipes);
		close(fd[0]);
		unlink_files(root);
		line = ft_strjoin((char *)list->next->content, "\n");
		free_command(&root);
		ft_lstclear(&tokens, free_token);
		free_string_array(context->environment_variables);
		free_string_array(context->dup_environment_variables);
		free_string_array(context->local_variables);
		write_temp_files(line, fd);
	}
	else
		ret_code = wait_for_heredoc_to_finish(list, fd, pid);
	return (ret_code);
}

int	open_temp_files(t_ast_node *root, t_list *tokens, t_list *list,
					t_variable_context *context)
{
	char	*redirect_type;
	int		ret_code;

	ret_code = 0;
	while (list != 0 && ret_code == 0)
	{
		redirect_type = (char *)list->content;
		if (ft_strncmp("<<", redirect_type, 3) == 0)
		{
			ret_code = fork_heredoc(root, tokens, list, context);
			if (ret_code == 2)
				return (ret_code);
		}
		list = list->next->next;
	}
	return (ret_code);
}

void	append_backslash_to_string_array(char **string_array)
{
	int		i;
	char	*string;

	i = 0;
	while (string_array[i])
	{
		string = string_array[i];
		string_array[i] = ft_strjoin(string, "/");
		free(string);
		i++;
	}
}

char	**extract_path_variable(char **environment_variable)
{
	char	**result;
	char	*key;
	char	*value;
	int		i;

	i = 0;
	key = 0;
	value = 0;
	while (environment_variable[i] && value == 0)
	{
		key = get_variable_key(environment_variable[i]);
		if (ft_strncmp("PATH", key, ft_strlen(key) + 1) == 0)
			value = get_variable_value(environment_variable[i]);
		free(key);
		i++;
	}
	if (value == 0)
		return (0);
	else
		result = ft_split(value, ':');
	append_backslash_to_string_array(result);
	free(value);
	return (result);
}

int	print_permission_denied_child(t_ast_node *root, t_list *tokens,
	char *string, t_variable_context *context)
{
	perror(string);
	unlink_files(root);
	free_command(&root);
	ft_lstclear(&tokens, free_token);
	free_string_array(context->environment_variables);
	free_string_array(context->local_variables);
	free_string_array(context->dup_environment_variables);
	close(0);
	close(1);
	close(2);
	exit(1);
}

void	compare_redirect_input_child(t_ast_node *root, t_list *tokens,
	t_list *redirection, t_variable_context *context)
{
	int	fd;

	if (ft_strncmp("<", (char *)redirection->content, 2) == 0)
	{
		fd = open((char *)redirection->next->content, O_RDONLY);
		if (fd == -1)
			print_permission_denied_child(root, tokens,
				(char *)redirection->next->content, context);
		else
		{
			dup2(fd, 0);
			close(fd);
		}
	}
}

void	dup_fd_to_stdout(int fd)
{
	dup2(fd, 1);
	close(fd);
}

void	process_other_redirect_child(t_ast_node *root, t_list *tokens,
										t_list *redirection,
										t_variable_context *context)
{
	int	fd;

	if (ft_strncmp(">>", (char *)redirection->content, 3) == 0)
	{
		fd = open((char *)redirection->next->content,
				O_CREAT | O_APPEND | O_WRONLY, 0644);
		if (fd == -1)
			print_permission_denied_child(root, tokens,
				(char *)redirection->next->content, context);
		else
			dup_fd_to_stdout(fd);
	}
	else if (ft_strncmp(">", (char *)redirection->content, 2) == 0)
	{
		fd = open((char *)redirection->next->content,
				O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if (fd == -1)
			print_permission_denied_child(root, tokens,
				(char *)redirection->next->content, context);
		else
			dup_fd_to_stdout(fd);
	}
	else
		compare_redirect_input_child(root, tokens, redirection, context);
}

void	exit_ambiguous_redirect(t_ast_node *root, t_list *tokens,
								t_list *redirection,
								t_variable_context *context)
{
	close(2);
	close(1);
	close(0);
	unlink_files(root);
	free_command(&root);
	ft_lstclear(&tokens, free_token);
	free_string_array(context->environment_variables);
	free_string_array(context->local_variables);
	free_string_array(context->dup_environment_variables);
	ft_putstr_fd((char *)redirection->next->content, 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
	exit(1);
}

void	configure_redirection_child(t_ast_node *root, t_list *tokens,
									t_list *redirection,
									t_variable_context *context)
{
	int	heredoc;

	while (redirection)
	{
		heredoc = ft_atoi((char *)redirection->content);
		if (heredoc != 0)
		{
			dup2(heredoc, 0);
			close(heredoc);
		}
		else if (!is_wildcard_present((char *)redirection->next->content))
			process_other_redirect_child(root, tokens, redirection, context);
		else if (is_wildcard_present((char *)redirection->next->content))
			exit_ambiguous_redirect(root, tokens, redirection, context);
		redirection = redirection->next->next;
	}
}

void	execute_with_execve(t_list *command, char **envp)
{
	int		i;
	char	**argv;

	argv = (char **)ft_calloc(sizeof(char *), ft_lstsize(command) + 1);
	i = 0;
	while (command)
	{
		argv[i++] = (char *)command->content;
		command = command->next;
	}
	execve(argv[0], argv, envp);
	exit(127);
}

void	print_no_file_or_directory(char *command,
		t_list *tokens, t_ast_node *root,
		t_variable_context *context)
{
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	free(command);
	unlink_files(root);
	free_command(&root);
	ft_lstclear(&tokens, free_token);
	free_string_array(context->environment_variables);
	free_string_array(context->local_variables);
	free_string_array(context->dup_environment_variables);
	close(0);
	close(1);
	close(2);
	exit(127);
}

void	substitute_node_with_list(t_list **result,
		t_list **prev, t_list **command)
{
	t_list	*match;

	match = find_match_string((char *)(*command)->content);
	if (match == 0)
	{
		*prev = *command;
		*command = (*command)->next;
		return ;
	}
	if (*prev == 0)
	{
		*result = match;
		*prev = ft_lstlast(match);
	}
	else
	{
		(*prev)->next = match;
		*prev = ft_lstlast(*prev);
	}
	(*prev)->next = (*command)->next;
	ft_lstdelone(*command, free);
	*command = (*prev)->next;
}

t_list	*expand_command_wildcard(t_list *command)
{
	t_list	*result;
	t_list	*prev;

	prev = 0;
	result = command;
	while (command)
	{
		if (is_wildcard_present((char *)command->content))
			substitute_node_with_list(&result, &prev, &command);
		else
		{
			prev = command;
			command = command->next;
		}
	}
	return (result);
}

void	search_and_exec(t_ast_node *root,
		t_list *tokens, t_ast_node *node,
		t_variable_context *context)
{
	char	**path_values;
	char	*path;
	char	*command_name;
	int		search;

	configure_redirection_child(root, tokens, node->redirection, context);
	if (access((char *)node->command->content, X_OK) == 0)
		execute_with_execve(node->command, context->environment_variables);
	command_name = ft_strdup((char *)node->command->content);
	path_values = extract_path_variable(context->environment_variables);
	if (path_values == 0)
		print_no_file_or_directory(command_name, tokens, root, context);
	search = 0;
	while (path_values[search])
	{
		path = ft_strjoin(path_values[search++], command_name);
		free(node->command->content);
		node->command->content = path;
		if (access(path, X_OK) == 0)
			execute_with_execve(node->command, context->environment_variables);
	}
	free_string_array(path_values);
	print_no_file_or_directory(command_name, tokens, root, context);
}

int	fork_and_wait(t_ast_node *root,
		t_list *tokens, t_ast_node *node,
		t_variable_context *context)
{
	int	pid;
	int	ret_code;

	ret_code = 0;
	pid = fork();
	if (pid == 0)
		search_and_exec(root, tokens, node, context);
	else
		waitpid(pid, &ret_code, 0);
	if (WIFEXITED(ret_code))
		return (WEXITSTATUS(ret_code));
	if (WIFSIGNALED(ret_code))
		return (128 + WTERMSIG(ret_code));
	return (ret_code);
}

char	is_builtin_command(char *command)
{
	return (ft_strncmp("echo", command, ft_strlen("echo") + 1) == 0
		|| ft_strncmp("cd", command, ft_strlen("cd") + 1) == 0
		|| ft_strncmp("pwd", command, ft_strlen("pwd") + 1) == 0
		|| ft_strncmp("export", command, ft_strlen("export") + 1) == 0
		|| ft_strncmp("unset", command, ft_strlen("unset") + 1) == 0
		|| ft_strncmp("env", command, ft_strlen("env") + 1) == 0
		|| ft_strncmp("exit", command, ft_strlen("exit") + 1) == 0);
}

int	ambigous_redirect_error(char *string)
{
	ft_putstr_fd(string, 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
	return (1);
}

char	print_permission_denied_root(char *string)
{
	perror(string);
	return (0);
}

int	open_files(t_list *redir)
{
	int	fd;

	while (redir)
	{
		if (ft_strncmp(">>", (char *)redir->content, 3) == 0
			|| ft_strncmp(">", (char *)redir->content, 2) == 0)
		{
			if (is_wildcard_present((char *)redir->next->content))
				return (ambigous_redirect_error((char *)
						redir->next->content));
			fd = open((char *)redir->next->content, O_CREAT | O_WRONLY, 0644);
			if (fd == -1)
			{
				print_permission_denied_root((char *)redir->next->content);
				break ;
			}
			close(fd);
		}
		else if (ft_strncmp("<", (char *)redir->content, 2) == 0
			&& is_wildcard_present((char *)redir->next->content))
			return (ambigous_redirect_error((char *)
					redir->next->content));
		redir = redir->next->next;
	}
	return (0);
}

int	append_local_variables_2(t_list *assignment,
		t_list *redirection, t_variable_context *context)
{
	int		seek;
	char	**new_array;

	new_array = (char **)ft_calloc(sizeof(char *),
			ft_size(context->local_variables)
			+ (ft_lstsize(assignment) - 1) + 1);
	seek = 0;
	while (context->local_variables && context->local_variables[seek])
	{
		new_array[seek] = ft_strdup(context->local_variables[seek]);
		seek++;
	}
	if (ft_strncmp(assignment->content, "export", 7) == 0)
		assignment = assignment->next;
	while (assignment)
	{
		new_array[seek] = ft_strdup(assignment->content);
		seek++;
		assignment = assignment->next;
	}
	free_string_array(context->local_variables);
	context->local_variables = new_array;
	return (open_files(redirection));
}

int	append_local_variables(t_list *assignment,
		t_list *redirection, t_variable_context *context)
{
	int		seek;
	char	**new_array;
	char	*string;

	new_array = (char **)ft_calloc(sizeof(char *),
			ft_size(context->local_variables)
			+ (ft_lstsize(assignment) >> 1) + 1);
	seek = 0;
	while (context->local_variables && context->local_variables[seek])
	{
		new_array[seek] = ft_strdup(context->local_variables[seek]);
		seek++;
	}
	while (assignment)
	{
		string = ft_strjoin((char *)assignment->content, "=");
		new_array[seek] = ft_strjoin(string, (char *)assignment->next->content);
		ft_add_to_others(new_array[seek], context);
		free(string);
		seek++;
		assignment = assignment->next->next;
	}
	free_string_array(context->local_variables);
	context->local_variables = new_array;
	return (open_files(redirection));
}

char	compare_redirect_input_root(t_list *redirection)
{
	int	fd;

	if (ft_strncmp("<", (char *)redirection->content, 2) == 0)
	{
		fd = open((char *)redirection->next->content, O_RDONLY);
		if (fd == -1)
			return (print_permission_denied_root(
					(char *)redirection->next->content));
		else
		{
			dup2(fd, 0);
			close(fd);
		}
	}
	return (1);
}

char	process_other_redirect_root(t_list *redirection)
{
	int	fd;

	if (ft_strncmp(">>", (char *)redirection->content, 3) == 0)
	{
		fd = open((char *)redirection->next->content,
				O_CREAT | O_APPEND | O_WRONLY, 0644);
		if (fd == -1)
			return (print_permission_denied_root(
					(char *)redirection->next->content));
		else
			dup_fd_to_stdout(fd);
	}
	else if (ft_strncmp(">", (char *)redirection->content, 2) == 0)
	{
		fd = open((char *)redirection->next->content,
				O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if (fd == -1)
			return (print_permission_denied_root(
					(char *)redirection->next->content));
		else
			dup_fd_to_stdout(fd);
	}
	else
		return (compare_redirect_input_root(redirection));
	return (1);
}

char	configure_redirection_root(t_list *redirection)
{
	int	heredoc;
	int	status;

	status = 1;
	while (redirection && status)
	{
		heredoc = ft_atoi((char *)redirection->content);
		if (heredoc != 0)
		{
			dup2(heredoc, 0);
			close(heredoc);
		}
		else if (!is_wildcard_present((char *)redirection->next->content))
			status = process_other_redirect_root(redirection);
		else if (is_wildcard_present((char *)redirection->next->content))
		{
			ft_putstr_fd((char *)redirection->next->content, 2);
			ft_putstr_fd(": ambiguous redirect\n", 2);
			return (0);
		}
		redirection = redirection->next->next;
	}
	return (status);
}

void	save_stdio(void)
{
	dup2(1, 1023);
	dup2(0, 1022);
}

void	restore_stdio(void)
{
	dup2(1023, 1);
	dup2(1022, 0);
	close(1023);
	close(1022);
}

void	decode_buildin_command(t_ast_node *root,
			t_list *token, t_ast_node *node,
			t_variable_context *context)
{
	if (ft_strncmp(node->command->content, "echo",
			ft_strlen(node->command->content) + 1) == 0)
		ft_echo(node->command);
	if (ft_strncmp(node->command->content, "cd",
			ft_strlen(node->command->content) + 1) == 0)
		ft_cd(node->command, context);
	if (ft_strncmp(node->command->content, "pwd",
			ft_strlen(node->command->content) + 1) == 0)
		ft_pwd();
	if (ft_strncmp(node->command->content, "export",
			ft_strlen(node->command->content) + 1) == 0)
		ft_export(node->command, node->redirection, context);
	if (ft_strncmp(node->command->content, "unset",
			ft_strlen(node->command->content) + 1) == 0)
		ft_unset(node->command, context);
	if (ft_strncmp(node->command->content, "env",
			ft_strlen(node->command->content) + 1) == 0)
		ft_env(context);
	if (ft_strncmp(node->command->content, "exit",
			ft_strlen(node->command->content) + 1) == 0)
		ft_exit(root, token, node, context);
}

int	execute_buildin_command(t_ast_node *root,
		t_list *token, t_ast_node *node,
		t_variable_context *context)
{
	if (node->redirection)
		save_stdio();
	if (node->redirection && configure_redirection_root(node->redirection) == 0)
	{
		restore_stdio();
		return (1);
	}
	decode_buildin_command(root, token, node, context);
	if (node->redirection)
		restore_stdio();
	return (0);
}

int	execute_command(t_ast_node *root,
		t_list *token, t_ast_node *node,
		t_variable_context *context)
{
	int	ret_code;

	if (node == 0)
		exit(0);
	node->command = expand_command_wildcard(node->command);
	ret_code = 0;
	if (node->command == 0 && node->assignment != 0)
		ret_code = append_local_variables(node->assignment,
				node->redirection, context);
	else if (node->command != 0
		&& is_builtin_command((char *)node->command->content))
		ret_code = execute_buildin_command(root,
				token, node, context);
	else if (node->command != 0
		&& !is_builtin_command((char *)node->command->content))
		ret_code = fork_and_wait(root,
				token, node, context);
	else if (node->command == 0
		&& node->assignment == 0 && node->redirection != 0)
		ret_code = open_files(node->redirection);
	return (ret_code);
}

int	execute_heredoc(t_ast_node *root, t_list *tokens,
		t_state_context *state_context, int ret_code)
{
	t_ast_node	*current;

	if (ret_code != 0)
		return (ret_code);
	if (state_context->current == 0
		|| (WIFEXITED(ret_code) && WEXITSTATUS(ret_code) == 2))
		return (ret_code);
	if (state_context->current->node != 0)
	{
		current = state_context->current;
		state_context->current = current->left;
		ret_code = execute_heredoc(root, tokens, state_context, ret_code);
		if (ret_code != 0)
			return (ret_code);
		state_context->current = current->right;
		ret_code = execute_heredoc(root, tokens, state_context, ret_code);
	}
	if (state_context->current->redirection != 0)
		ret_code = open_temp_files(root, tokens,
				state_context->current->redirection, state_context->context);
	return (ret_code);
}

unsigned char	execute_logical_or(t_ast_node *root,
		t_list *tokens,
		t_ast_node *node,
		t_variable_context *context)
{
	int	ret_code;

	ret_code = execute_command_ast(root, tokens, node->left, context);
	if (ret_code < 128)
	{
		if (ret_code == 0)
			return (ret_code);
		ret_code = execute_command_ast(root, tokens, node->right, context);
		return (ret_code);
	}
	return (ret_code);
}

unsigned char	execute_logical_and(t_ast_node *root,
		t_list *tokens,
		t_ast_node *node,
		t_variable_context *context)
{
	int	ret_code;

	ret_code = execute_command_ast(root, tokens, node->left, context);
	if (ret_code < 128)
	{
		if (ret_code != 0)
			return (ret_code);
		ret_code = execute_command_ast(root, tokens, node->right, context);
		return (ret_code);
	}
	return (ret_code);
}

void	exec_pipe_left_child(t_parse_context *parse_context,
		int *fd, t_ast_node *node, t_variable_context *context)
{
	int	ret_code;

	close(fd[0]);
	close(1);
	dup2(fd[1], 1);
	close(fd[1]);
	ret_code = execute_command_ast(parse_context->root,
			parse_context->tokens, node->left, context);
	unlink_files(parse_context->root);
	free_command(&parse_context->root);
	ft_lstclear(&parse_context->tokens, free_token);
	free_string_array(context->environment_variables);
	free_string_array(context->local_variables);
	free_string_array(context->dup_environment_variables);
	close(2);
	close(1);
	close(0);
	exit(ret_code);
}

void	exec_pipe_right_child(t_parse_context *parse_context,
		int *fd, t_ast_node *node, t_variable_context *context)
{
	int	ret_code;

	close(fd[1]);
	close(0);
	dup2(fd[0], 0);
	close(fd[0]);
	ret_code = execute_command_ast(parse_context->root,
			parse_context->tokens, node->right, context);
	unlink_files(parse_context->root);
	free_command(&parse_context->root);
	ft_lstclear(&parse_context->tokens, free_token);
	free_string_array(context->environment_variables);
	free_string_array(context->local_variables);
	free_string_array(context->dup_environment_variables);
	close(2);
	close(1);
	close(0);
	exit(ret_code);
}

void	close_all_pipes(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}

int	*prepare_pipe_fd(t_ast_node *root, t_ast_node *node,
		t_list *tokens, t_variable_context *context)
{
	int					*pid;
	int					fd[2];
	t_parse_context		parse_context;

	parse_context.root = root;
	parse_context.tokens = tokens;
	pipe(fd);
	pid = (int *)ft_calloc(sizeof(int), 2);
	pid[0] = fork();
	if (pid[0] == 0)
	{
		free(pid);
		exec_pipe_left_child(&parse_context, fd, node, context);
	}
	else
		pid[1] = fork();
	if (pid[1] == 0)
	{
		free(pid);
		exec_pipe_right_child(&parse_context, fd, node, context);
	}
	else
		close_all_pipes(fd);
	return (pid);
}

unsigned char	execute_pipe(t_ast_node *root,
		t_list *tokens, t_ast_node *node,
		t_variable_context *context)
{
	int	*pid;
	int	ret_code;
	int	proc_count;

	pid = prepare_pipe_fd(root, node, tokens, context);
	proc_count = 0;
	while (proc_count < 2)
		waitpid(pid[proc_count++], &ret_code, 0);
	free(pid);
	if (WIFEXITED(ret_code))
		return (WEXITSTATUS(ret_code));
	else if (WIFSIGNALED(ret_code))
		return (128 + WTERMSIG(ret_code));
	return ((unsigned char)255);
}

int	execute_command_ast(t_ast_node *root,
	t_list *tokens,
	t_ast_node *node,
	t_variable_context *context)
{
	int	ret_code;

	ret_code = 255;
	if (node == 0)
		return (0);
	if (node->node == 0)
		ret_code = execute_command(root, tokens, node, context);
	else if (node->node->type == LOGICAL_OR)
		ret_code = execute_logical_or(root, tokens, node, context);
	else if (node->node->type == LOGICAL_AND)
		ret_code = execute_logical_and(root, tokens, node, context);
	else if (node->node->type == PIPE)
		ret_code = execute_pipe(root, tokens, node, context);
	return (ret_code);
}

void	unlink_temp_files(t_list *list)
{
	while (list != 0)
	{
		if (ft_atoi((char *)list->content) != 0)
			close(ft_atoi((char *)list->content));
		list = list->next->next;
	}
}

void	unlink_files(t_ast_node *node)
{
	if (node == 0)
		return ;
	if (node->node != 0)
	{
		unlink_files(node->left);
		unlink_files(node->right);
	}
	if (node->redirection != 0)
		unlink_temp_files(node->redirection);
}
