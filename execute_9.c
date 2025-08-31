/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_9.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:07:09 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/29 23:22:09 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

extern int	g_ret_code;

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
