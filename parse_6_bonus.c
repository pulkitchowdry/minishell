/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_6_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:13:30 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/31 15:22:09 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell_bonus.h"

#include <stdlib.h>

extern int	g_ret_code;

t_ast_node	*parse_command(t_list **list, t_state_context *context)
{
	t_ast_node	*current;
	t_token		*cur_tok;

	if (*list == 0)
		return (0);
	current = 0;
	cur_tok = (t_token *)(*list)->content;
	while (cur_tok != 0 && cur_tok->type == SPACES)
		cur_tok = advance_new_node(list);
	if (cur_tok != 0 && cur_tok->type == OPEN_BRACKET)
		return (parse_bracket(list, context));
	else if (cur_tok != 0 && is_extractable(cur_tok->type))
		current = extract_command(list, context);
	return (current);
}

t_ast_node	*parse_pipeline(t_list **list, t_state_context *context)
{
	t_token		*cur_tok;

	context->current = parse_command(list, context);
	if (context->current == 0)
		return (0);
	if (*list == 0)
		return (context->current);
	cur_tok = (t_token *)(*list)->content;
	while (cur_tok != 0 && (cur_tok->type == SPACES || cur_tok->type == PIPE))
	{
		while (cur_tok != 0 && cur_tok->type == SPACES)
			cur_tok = advance_new_node(list);
		if (cur_tok == 0)
			return (0);
		if (cur_tok->type == PIPE)
			context->current = parse_new_node(context,
					cur_tok, list, parse_command);
		if (*list == 0)
			break ;
		cur_tok = (t_token *)(*list)->content;
	}
	return (context->current);
}

char	is_space_or_logical(t_token *cur_tok)
{
	return (cur_tok->type == SPACES || cur_tok->type == LOGICAL_OR
		|| cur_tok->type == LOGICAL_AND);
}

t_ast_node	*parse_list(t_list **list, t_state_context *context)
{
	t_token		*cur_tok;

	context->current = parse_pipeline(list, context);
	if (context->current == 0)
		return (0);
	if (*list == 0)
		return (context->current);
	cur_tok = (t_token *)(*list)->content;
	while (cur_tok->type == SPACES || cur_tok->type == LOGICAL_OR
		|| cur_tok->type == LOGICAL_AND)
	{
		while (cur_tok != 0 && cur_tok->type == SPACES)
			cur_tok = advance_new_node(list);
		if (cur_tok == 0)
			return (0);
		if (cur_tok->type == LOGICAL_AND || cur_tok->type == LOGICAL_OR)
			context->current = parse_new_node(context, cur_tok,
					list, parse_pipeline);
		if (*list == 0)
			break ;
		cur_tok = (t_token *)(*list)->content;
	}
	return (context->current);
}
