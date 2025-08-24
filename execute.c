/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chikoh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 20:40:39 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/17 17:40:31 by chikoh           ###   ########.fr       */
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
