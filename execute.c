/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chikoh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 20:40:39 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/17 22:21:10 by chikoh           ###   ########.fr       */
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

char	execute_logical_or(t_ast_node *node)
{
	int	pid;
	int	ret_code;

	pid = fork();
	if (pid == 0)
		execute_command(node->left);
	else
		wait_pid(pid, &ret_code, 0);
	if (WIFEXITED(ret_code))
	{
		if (WEXITSTATUS(ret_code) == 0)
			return (WEXITSTATUS(ret_code));
		pid = fork();
		if (pid == 0)
			execute_command(node->right);
		else
			wait_pid(pid, &ret_code, 0);
		if (WIFEXITED(ret_code))
			return (WEXITSTATUS(ret_code));
		else if (WIFSIGNALED(ret_code))
			return (128 + WTERMSIG(ret_code));
	}
	else if (WIFSIGNALED(ret_code))
		return (128 + WTERMSIG(ret_code));
	return (255);
}

char	execute_logical_and(t_ast_node *node)
{
	int	pid;
	int	ret_code;

	pid = fork();
	if (pid == 0)
		execute_command(node->left);
	else
		wait_pid(pid, &ret_code, 0);
	if (WIFEXITED(ret_code))
	{
		if (WEXITSTATUS(ret_code) != 0)
			return (WEXITSTATUS(ret_code));
		pid = fork();
		if (pid == 0)
			execute_command(node->right);
		else
			wait_pid(pid, &ret_code, 0);
		if (WIFEXITED(ret_code))
			return (WEXITSTATUS(ret_code));
		else if (WIFSIGNALED(ret_code))
			return (128 + WTERMSIG(ret_code));
	}
	else if (WIFSIGNALED(ret_code))
		return (128 + WTERMSIG(ret_code));
	return (255);
}

char	execute_pipe(t_ast_node *node)
{
	int	pid[2];
	int	ret_code;
	int	proc_count;

	pid[0] = fork();
	if (pid[0] == 0)
		execute_command(node->left);
	else
		pid[1] = fork();
	proc_count = 0;
	while (proc_count < 2)
	{
		wait_pid(pid[proc_count], &ret_code, 0);
		proc_count++;
	}
	if (WIFEXITED(ret_code))
		return (WEXITSTATUS(ret_code));
	else if (WIFSIGNALED(ret_code))
		return (128 + WTERMSIG(ret_code));
	return (255);
}

void	execute_command_ast(t_ast_node *node)
{
	if (node == 0)
		return ;
	if (node->node != 0)
		execute_command(node);
	else if (node->node->type == LOGICAL_OR)
		execute_logical_or(node);
	else if (node->node->type == LOGICAL_AND)
		execute_logical_and(node);
	else if (node->node->type == PIPE)
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
