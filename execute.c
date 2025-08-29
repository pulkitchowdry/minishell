/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 20:40:39 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/29 23:07:03 by pchowdry         ###   ########.fr       */
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
