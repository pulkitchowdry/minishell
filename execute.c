/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chikoh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 20:40:39 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/21 16:40:14 by chikoh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void	write_temp_files(char *delimiter, int fd)
{
	char	*line_string;
	char	*delimiter_with_newline;

	delimiter_with_newline = ft_strjoin(delimiter, "\n");
	ft_putstr_fd("> ", 1);
	line_string = get_next_line(0);
	while (line_string != 0 && ft_strncmp(line_string, delimiter_with_newline,
			ft_strlen(delimiter_with_newline) + 1) != 0)
	{
		write(fd, line_string, ft_strlen(line_string));
		free(line_string);
		ft_putstr_fd("> ", 1);
		line_string = get_next_line(0);
	}
	if ((line_string == 0 || ft_strncmp(line_string, "", 1) == 0)
		&& ft_strncmp(delimiter, "", 1) != 0)
		ft_putstr_fd("here-document delimited by end-of-file", 2);
	free(line_string);
	free(delimiter_with_newline);
}

void	open_temp_files(t_list *list)
{
	char					*redirect_type;
	char					*filename;
	char					*filename_tmp;
	int						fd;
	static unsigned int		index = 0;

	while (list != 0)
	{
		redirect_type = (char *)list->content;
		if (ft_strncmp("<<", redirect_type, 3) == 0)
		{
			filename = ft_itoa(index++);
			filename_tmp = ft_strjoin(filename, ".tmp");
			free(filename);
			fd = open(filename_tmp, O_WRONLY | O_CREAT | O_TRUNC, 0777);
			write_temp_files((char *)list->next->content, fd);
			close(fd);
			free(list->next->content);
			list->next->content = filename_tmp;
		}
		list = list->next->next;
	}
}

void	execute_command(t_ast_node *node)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	if (node == 0)
		exit(0);
	exit(0);
}

void	execute_heredoc(t_ast_node *node)
{
	if (node == 0)
		return ;
	if (node->node != 0)
	{
		execute_heredoc(node->left);
		execute_heredoc(node->right);
	}
	if (node->redirection != 0)
		open_temp_files(node->redirection);
}

char	execute_command_ast_child(t_ast_node *node, t_variable_context *context)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	return (execute_command_ast(node, context));
}

int	execute_and_wait_child(t_ast_node *node, t_variable_context *context)
{
	int	pid;
	int	ret_code;

	ret_code = -1;
	pid = fork();
	if (pid == 0)
		execute_command_ast_child(node, context);
	else
		waitpid(pid, &ret_code, 0);
	return (ret_code);
}

unsigned char	execute_logical_or(t_ast_node *node,
		t_variable_context *context)
{
	int	ret_code;

	ret_code = execute_and_wait_child(node->left, context);
	if (WIFEXITED(ret_code))
	{
		if (WEXITSTATUS(ret_code) == 0)
			return (WEXITSTATUS(ret_code));
		ret_code = execute_and_wait_child(node->right, context);
		if (WIFEXITED(ret_code))
			return (WEXITSTATUS(ret_code));
		else if (WIFSIGNALED(ret_code))
			return (128 + WTERMSIG(ret_code));
	}
	else if (WIFSIGNALED(ret_code))
		return (128 + WTERMSIG(ret_code));
	return ((unsigned char)255);
}

unsigned char	execute_logical_and(t_ast_node *node,
		t_variable_context *context)
{
	int	ret_code;

	ret_code = execute_and_wait_child(node->left, context);
	if (WIFEXITED(ret_code))
	{
		if (WEXITSTATUS(ret_code) != 0)
			return (WEXITSTATUS(ret_code));
		ret_code = execute_and_wait_child(node->right, context);
		if (WIFEXITED(ret_code))
			return (WEXITSTATUS(ret_code));
		else if (WIFSIGNALED(ret_code))
			return (128 + WTERMSIG(ret_code));
	}
	else if (WIFSIGNALED(ret_code))
		return (128 + WTERMSIG(ret_code));
	return (255);
}

void	exec_pipe_left_child(int *fd, t_ast_node *node, t_variable_context *context)
{
		close(fd[0]);
		close(1);
		dup2(fd[1], 1);
		close(fd[1]);
		execute_command_ast_child(node->left, context);
}

void	exec_pipe_right_child(int *fd, t_ast_node *node, t_variable_context *context)
{
		close(fd[1]);
		close(0);
		dup2(fd[0], 0);
		close(fd[0]);
		execute_command_ast_child(node->right, context);
}

void	close_all_pipes(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}

unsigned char	execute_pipe(t_ast_node *node, t_variable_context *context)
{
	int	pid[2];
	int	ret_code;
	int	proc_count;
	int	fd[2];

	pipe(fd);
	pid[0] = fork();
	if (pid[0] == 0)
		exec_pipe_left_child(fd, node, context);
	else
		pid[1] = fork();
	if (pid[1] == 0)
		exec_pipe_right_child(fd, node, context);
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

unsigned char	execute_command_ast(t_ast_node *node,
	t_variable_context *context)
{
	int	ret_code;

	ret_code = 255;
	if (node == 0)
		return (0);
	if (node->node != 0)
		execute_command(node);
	else if (node->node->type == LOGICAL_OR)
		ret_code = execute_logical_or(node, context);
	else if (node->node->type == LOGICAL_AND)
		ret_code = execute_logical_and(node, context);
	else if (node->node->type == PIPE)
		ret_code = execute_pipe(node, context);
	return (ret_code);
}

void	unlink_temp_files(t_list *list)
{
	char	*redirect_type;

	while (list != 0)
	{
		redirect_type = (char *)list->content;
		if (ft_strncmp("<<", redirect_type, 3) == 0)
			unlink((char *)list->next->content);
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
