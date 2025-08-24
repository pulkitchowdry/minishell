/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chikoh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 20:40:39 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/24 21:42:53 by chikoh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void	close_heredoc_pipes()
{
	close(1023);
	exit(2);
}

void	write_temp_files(char *delimiter_string, int* fd)
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
	close(1023);
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
		return (ret_code);
	}
	free(redirection_delimiter->content);
	redirection_delimiter->content = ft_itoa(fd[0]);
	return (ret_code);
}

int	fork_heredoc(t_ast_node *root, t_list *tokens, t_list *list)
{
	int		ret_code;
	int		fd[2];
	int		pid;
	char		*line;

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
		write_temp_files(line, fd);
	}
	else
		ret_code = wait_for_heredoc_to_finish(list, fd, pid);
	return (ret_code);
}

int	open_temp_files(t_ast_node *root, t_list *tokens, t_list *list)
{
	char	*redirect_type;
	int		ret_code;

	ret_code = 0;
	while (list != 0 && ret_code == 0)
	{
		redirect_type = (char *)list->content;
		if (ft_strncmp("<<", redirect_type, 3) == 0)
		{
			ret_code = fork_heredoc(root, tokens, list);
			if (WIFEXITED(ret_code) && WEXITSTATUS(ret_code) == 2)
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

int	print_permission_denied()
{
	close(0);
	close(1);
	exit(1);
}
 
void	compare_redirect_input(t_list *redirection)
{
	int	fd;

	if (ft_strncmp("<", (char *)redirection->content, 2) == 0)
	{
		fd = open((char *)redirection->next->content, O_RDONLY);
		if (fd == -1)
			print_permission_denied();
		dup2(fd, 0);
		close(fd);
	}
}

void	process_other_redirect(t_list *redirection)
{
	int	fd;

	if (ft_strncmp(">>", (char *)redirection->content, 3) == 0)
	{
		fd = open((char *)redirection->next->content, O_CREAT | O_APPEND | O_WRONLY);
		if (fd == -1)
			print_permission_denied();
		dup2(fd, 1);
		close(fd);
	}
	else if (ft_strncmp(">", (char *)redirection->content, 2) == 0)
	{
		fd = open((char *)redirection->next->content, O_CREAT | O_TRUNC | O_WRONLY);
		if (fd == -1)
			print_permission_denied();
		dup2(fd, 1);
		close(fd);
	}
	else
		compare_redirect_input(redirection);
}

void	configure_redirection(t_list *redirection)
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
		else
			process_other_redirect(redirection);
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

void	print_no_file_or_directory(char *command, t_ast_node *node, t_list *tokens)
{
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	free(command);
	free_command(&node);
	ft_lstclear(&tokens, free_token);
	close(0);
	close(1);
	close(2);
	exit(127);
}

void	search_and_exec(t_list *tokens, t_ast_node *node, t_variable_context *context)
{
	char	**path_values;
	char	*path;
	char	*command_name;
	int	search;

	configure_redirection(node->redirection);
	if (access((char *)node->command->content, X_OK) == 0)
		execute_with_execve(node->command, context->environment_variables);
	command_name = ft_strdup((char *)node->command->content);
	path_values = extract_path_variable(context->environment_variables);
	if (path_values == 0)
		print_no_file_or_directory(command_name, node, tokens);
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
	print_no_file_or_directory(command_name, node, tokens);
}

int	fork_and_wait(t_ast_node *root, t_list *tokens, t_ast_node *node, t_variable_context *context)
{
	int	pid;
	int	ret_code;

	ret_code = 0;
	pid = fork();
	if (pid == 0)
	{
		free_command_except_self(&root, node);
		search_and_exec(tokens, node, context);
	}
	else
		waitpid(pid, &ret_code, 0);
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

int	open_files(t_list *redirection)
{
	int	fd;

	while (redirection)
	{
		if (ft_strncmp(">>", (char *)redirection->content, 3) == 0
			|| ft_strncmp(">", (char *)redirection->content, 2) == 0)
		{
			fd = open((char *)redirection->next->content, O_CREAT | O_WRONLY);
			if (fd == -1)
			{
				print_permission_denied();
				break ;
			}
			close(fd);
		}
		redirection = redirection->next->next;
	}
	return (0);
}

int	append_local_variables(t_list *assignment, t_list *redirection, t_variable_context *context)
{
	int	seek;
	char	**new_array;
	char	*string;

	new_array = (char **)ft_calloc(sizeof(char *), ft_size(context->local_variables)
			+ (ft_lstsize(assignment) >> 1) + 1);
	seek = 0;
	while (context->local_variables[seek])
	{
		new_array[seek] = context->local_variables[seek];
		seek++;
	}
	while (assignment)
	{
		string = ft_strjoin((char *)assignment->content, "=");
		new_array[seek] = ft_strjoin(string, (char *)assignment->next->content);
		seek++;
		assignment = assignment->next->next;
	}
	free_string_array(context->local_variables);
	context->local_variables = new_array;
	return (open_files(redirection));
}

int	execute_buildin_command(t_list *command, t_list *redirection, t_variable_context *context)
{
	(void)command;
	(void)redirection;
	(void)context;
	return (0);
}

int	execute_command(t_ast_node *root, t_list *token, t_ast_node *node, t_variable_context *context)
{
	if (node == 0)
		exit(0);
	if (node->command == 0 && node->assignment != 0)
		return (append_local_variables(node->assignment, node->redirection, context));
	else if (node->command != 0 && is_builtin_command((char *)node->command->content))
		return (execute_buildin_command(node->command, node->redirection, context));
	else if (node->command != 0 && !is_builtin_command((char *)node->command->content))
		return (fork_and_wait(root, token, node, context));
	else if (node->command == 0 && node->assignment == 0 && node->redirection != 0)
		return (open_files(node->redirection));
	exit(0);
}

int	execute_heredoc(t_ast_node *root, t_list* tokens, t_ast_node *node, int ret_code)
{
	if (node == 0 || (WIFEXITED(ret_code) && WEXITSTATUS(ret_code) == 2))
		return (ret_code);
	if (node->node != 0)
	{
		ret_code = execute_heredoc(root, tokens, node->left, ret_code);
		ret_code = execute_heredoc(root, tokens, node->right, ret_code);
	}
	if (node->redirection != 0)
		ret_code = open_temp_files(root, tokens, node->redirection);
	return (ret_code);
}

unsigned char	execute_logical_or(t_ast_node *root,
		t_list	*tokens,
		t_ast_node *node,
		t_variable_context *context)
{
	int	ret_code;

	ret_code = execute_command_ast(root, tokens, node->left, context);
	if (WIFEXITED(ret_code))
	{
		if (WEXITSTATUS(ret_code) == 0)
			return (WEXITSTATUS(ret_code));
		ret_code = execute_command_ast(root, tokens, node->right, context);
		if (WIFEXITED(ret_code))
			return (WEXITSTATUS(ret_code));
		else if (WIFSIGNALED(ret_code))
			return (128 + WTERMSIG(ret_code));
	}
	else if (WIFSIGNALED(ret_code))
		return (128 + WTERMSIG(ret_code));
	return ((unsigned char)255);
}

unsigned char	execute_logical_and(t_ast_node *root,
		t_list *tokens,
		t_ast_node *node,
		t_variable_context *context)
{
	int	ret_code;

	ret_code = execute_command_ast(root, tokens, node->left, context);
	if (WIFEXITED(ret_code))
	{
		if (WEXITSTATUS(ret_code) != 0)
			return (WEXITSTATUS(ret_code));
		ret_code = execute_command_ast(root, tokens, node->right, context);
		if (WIFEXITED(ret_code))
			return (WEXITSTATUS(ret_code));
		else if (WIFSIGNALED(ret_code))
			return (128 + WTERMSIG(ret_code));
	}
	else if (WIFSIGNALED(ret_code))
		return (128 + WTERMSIG(ret_code));
	return (255);
}

void	exec_pipe_left_child(t_list *tokens, int *fd, t_ast_node *node, t_variable_context *context)
{
	close(fd[0]);
	close(1);
	dup2(fd[1], 1);
	close(fd[1]);
	execute_command_ast(node, tokens, node->left, context);
}

void	exec_pipe_right_child(t_list *tokens, int *fd, t_ast_node *node, t_variable_context *context)
{
	close(fd[1]);
	close(0);
	dup2(fd[0], 0);
	close(fd[0]);
	execute_command_ast(node, tokens, node->right, context);
}

void	close_all_pipes(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}

unsigned char	execute_pipe(t_ast_node *root, t_list *tokens, t_ast_node *node, t_variable_context *context)
{
	int	pid[2];
	int	ret_code;
	int	proc_count;
	int	fd[2];

	pipe(fd);
	free_command_except_self(&root, node);
	pid[0] = fork();
	if (pid[0] == 0)
		exec_pipe_left_child(tokens, fd, node, context);
	else
		pid[1] = fork();
	if (pid[1] == 0)
		exec_pipe_right_child(tokens, fd, node, context);
	else
		close_all_pipes(fd);
	proc_count = 0;
	while (proc_count < 2)
		waitpid(pid[proc_count++], &ret_code, 0);
	if (WIFEXITED(ret_code))
		return (WEXITSTATUS(ret_code));
	else if (WIFSIGNALED(ret_code))
		return (128 + WTERMSIG(ret_code));
	return ((unsigned char)255);
}

unsigned char	execute_command_ast(t_ast_node *root,
	t_list *tokens,
	t_ast_node *node,
	t_variable_context *context)
{
	int	ret_code;

	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	ret_code = 255;
	if (node == 0)
		return (0);
	if (node->node == 0)
		execute_command(root, tokens, node, context);
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
