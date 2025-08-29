/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:03:09 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/29 23:22:44 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

extern int	g_ret_code;

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
