/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:05:22 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/29 23:22:29 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

extern int	g_ret_code;

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
