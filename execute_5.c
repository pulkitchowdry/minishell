/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:04:25 by pchowdry          #+#    #+#             */
/*   Updated: 2025/09/02 14:46:10 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

extern int	g_ret_code;

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

int	append_local_variables_2(t_list *asgmt,
		t_list *redirection, t_variable_context *context)
{
	int		seek;
	char	**new_array;

	new_array = (char **)ft_calloc(sizeof(char *),
			ft_size(context->local_variables)
			+ (ft_lstsize(asgmt) - 1) + 1);
	seek = 0;
	while (context->local_variables && context->local_variables[seek])
	{
		new_array[seek] = ft_strdup(context->local_variables[seek]);
		seek++;
	}
	if (ft_strncmp(asgmt->content, "export", 7) == 0)
		asgmt = asgmt->next;
	while (asgmt && ft_asgmt(asgmt->content, 1) && ft_asgmt(asgmt->content, 2)
		&& ft_isalpha(((char *)asgmt->content)[0])
			&& (((char *)asgmt->content)[0] != '_'))
	{
		new_array[seek] = ft_strdup(asgmt->content);
		seek++;
		asgmt = asgmt->next;
	}
	free_string_array(context->local_variables);
	context->local_variables = new_array;
	return (open_files(redirection));
}
