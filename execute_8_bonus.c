/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_8_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:06:32 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/31 15:21:30 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

extern int	g_ret_code;

int	execute_logical_or(t_ast_node *root,
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

int	execute_logical_and(t_ast_node *root,
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
