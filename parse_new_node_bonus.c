/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_new_node_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:56:58 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/31 15:22:15 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell_bonus.h"

extern int	g_ret_code;

t_ast_node	*parse_new_node(t_state_context *state_context,
		t_token *cur_tok, t_list **list,
		t_ast_node *(*func)(t_list **, t_state_context *))
{
	t_ast_node	*current;

	current = (t_ast_node *)ft_calloc(sizeof(t_ast_node), 1);
	current->node = cur_tok;
	current->left = state_context->current;
	*list = (*list)->next;
	current->right = func(list, state_context);
	return (current);
}

char	is_extractable(int type)
{
	return (type == STRING || type == DOUBLE_QUOTE_STRING
		|| type == SINGLE_QUOTE_STRING || type == HERE_DOC
		|| type == REDIRECT_APPEND || type == REDIRECT_OUTPUT
		|| type == REDIRECT_INPUT || type == SPACES
		|| type == ASSIGNMENT || type == VARIABLE
		|| type == WILDCARD || type == ARGUMENTS
		|| type == COMMAND);
}

t_token	*advance_new_node(t_list **list)
{
	*list = (*list)->next;
	if (*list != 0)
		return ((t_token *)(*list)->content);
	else
		return (0);
}

t_ast_node	*free_and_return_zero(t_ast_node *current)
{
	free_command(&current);
	return (0);
}

t_ast_node	*parse_bracket(t_list **list, t_state_context *context)
{
	t_ast_node	*current;
	t_token		*cur_tok;

	*list = (*list)->next;
	current = parse_list(list, context);
	if (*list == 0)
		return (free_and_return_zero(current));
	cur_tok = (t_token *)(*list)->content;
	while (cur_tok->type == SPACES)
		cur_tok = advance_new_node(list);
	if (cur_tok->type == CLOSE_BRACKET)
		*list = (*list)->next;
	else
		free_command(&current);
	return (current);
}
