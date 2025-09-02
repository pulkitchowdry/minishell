/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_7_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:05:58 by pchowdry          #+#    #+#             */
/*   Updated: 2025/09/02 13:54:16 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

extern int	g_ret_code;

void	restore_stdio(void)
{
	dup2(1023, 1);
	dup2(1022, 0);
	close(1023);
	close(1022);
}

int	decode_buildin_command(t_ast_node *root,
			t_list *token, t_ast_node *node,
			t_variable_context *context)
{
	if (ft_strncmp(node->command->content, "echo",
			ft_strlen(node->command->content) + 1) == 0)
		return (ft_echo(node->command));
	if (ft_strncmp(node->command->content, "cd",
			ft_strlen(node->command->content) + 1) == 0)
		return (ft_cd(node->command, context));
	if (ft_strncmp(node->command->content, "pwd",
			ft_strlen(node->command->content) + 1) == 0)
		return (ft_pwd());
	if (ft_strncmp(node->command->content, "export",
			ft_strlen(node->command->content) + 1) == 0)
		return (ft_export(node->command, node->redirection, context));
	if (ft_strncmp(node->command->content, "unset",
			ft_strlen(node->command->content) + 1) == 0)
		return (ft_unset(node->command, context));
	if (ft_strncmp(node->command->content, "env",
			ft_strlen(node->command->content) + 1) == 0)
		return (ft_env(context));
	if (ft_strncmp(node->command->content, "exit",
			ft_strlen(node->command->content) + 1) == 0)
		return (ft_exit(root, token, node, context));
	return (0);
}

int	execute_buildin_command(t_ast_node *root,
		t_list *token, t_ast_node *node,
		t_variable_context *context)
{
	int	ret_code;

	if (node->redirection)
		save_stdio();
	if (node->redirection && configure_redirection_root(node->redirection) == 0)
	{
		restore_stdio();
		return (1);
	}
	ret_code = decode_buildin_command(root, token, node, context);
	if (node->redirection)
		restore_stdio();
	return (ret_code);
}

int	execute_command(t_ast_node *root,
		t_list *token, t_ast_node *node,
		t_variable_context *context)
{
	int	ret_code;

	if (node == 0)
		exit(0);
	node->command = expand_command_wildcard_and_quotes(node->command, context);
	ret_code = 0;
	if (node->command == 0 && node->assignment != 0)
		ret_code = append_local_variables(node->assignment,
				node->redirection, context);
	else if (node->command != 0
		&& is_builtin_command((char *)node->command->content))
		ret_code = execute_buildin_command(root,
				token, node, context);
	else if (node->command != 0
		&& !is_builtin_command((char *)node->command->content))
		ret_code = fork_and_wait(root,
				token, node, context);
	else if (node->command == 0
		&& node->assignment == 0 && node->redirection != 0)
		ret_code = open_files(node->redirection);
	return (ret_code);
}

int	execute_heredoc(t_ast_node *root, t_list *tokens,
		t_state_context *state_context, int ret_code)
{
	t_ast_node	*current;

	if (ret_code != 0)
		return (ret_code);
	if (state_context->current == 0
		|| (WIFEXITED(ret_code) && WEXITSTATUS(ret_code) == 2))
		return (ret_code);
	if (state_context->current->node != 0)
	{
		current = state_context->current;
		state_context->current = current->left;
		ret_code = execute_heredoc(root, tokens, state_context, ret_code);
		if (ret_code != 0)
			return (ret_code);
		state_context->current = current->right;
		ret_code = execute_heredoc(root, tokens, state_context, ret_code);
	}
	if (state_context->current->redirection != 0)
		ret_code = open_temp_files(root, tokens,
				state_context->current->redirection, state_context->context);
	return (ret_code);
}
