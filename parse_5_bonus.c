/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_5_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:12:53 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/31 15:22:06 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell_bonus.h"

#include <stdlib.h>

extern int	g_ret_code;

int	process_redirect(t_list **list, t_token *cur_tok,
		t_ast_node **current, int current_state)
{
	t_token	*next_tok;

	ft_lstadd_back(&((*current)->redirection),
		ft_lstnew(ft_strdup(cur_tok->string)));
	ft_lstadd_back(&((*current)->redirection), ft_lstnew(ft_strdup("")));
	if (*list == 0)
	{
		free_command(current);
		return (EXIT);
	}
	next_tok = (t_token *)(*list)->content;
	if (next_tok->type == SPACES)
		return (determine_next_redirect_if_space(current_state));
	else if (is_command_string(next_tok->type))
		return (determine_next_redirect_if_command_string(current_state));
	else if (is_redirect(next_tok->type))
		return (determine_next_redirect_string_if_redirect(current_state));
	else
	{
		free_command(current);
		return (EXIT);
	}
}

int	process_command_string(t_list **list, t_token *cur_tok,
		t_ast_node **current, t_variable_context *context)
{
	t_token	*next_tok;

	if (is_command_string(cur_tok->type))
		append_string_to_current_node(cur_tok, (*current)->command,
			context);
	if (*list == 0)
		return (EXIT);
	next_tok = (t_token *)(*list)->content;
	if (is_command_string(next_tok->type))
		return (COMMAND_STRING);
	else if (is_redirect(next_tok->type))
		return (COMMAND_REDIRECT);
	else if (next_tok->type == SPACES)
		return (COMMAND_SPACE);
	else if (is_pipe_or_logical(next_tok->type))
		return (EXIT);
	else
	{
		free_command(current);
		return (EXIT);
	}
}

int	process_redirect_string(t_list **list, t_token *cur_tok,
		int current_state, t_state_context *state_context)
{
	t_token	*next_tok;

	if (is_command_string(cur_tok->type))
		append_string_to_current_node(cur_tok,
			(*state_context->current).redirection, state_context->context);
	if (*list == 0)
		return (EXIT);
	next_tok = (t_token *)(*list)->content;
	if (next_tok->type == SPACES)
		return (determine_next_redirect_string_if_space(current_state));
	else if (is_command_string(next_tok->type))
		return (determine_next_redirect_string_if_command(current_state));
	else if (is_redirect(next_tok->type))
		return (determine_next_redirect_string_if_redirect(current_state));
	else if (is_pipe_or_logical(next_tok->type)
		|| next_tok->type == CLOSE_BRACKET)
		return (EXIT);
	else
		return (EXIT);
}

int	process_redirect_space(t_list **list,
		t_ast_node **current, int current_state)
{
	if (*list == 0)
	{
		free_command(current);
		return (EXIT);
	}
	if (((t_token *)(*list)->content)->type == SPACES)
		return (determine_next_redirect_space_if_space(current_state));
	else if (is_command_string(((t_token *)(*list)->content)->type))
		return (determine_next_redirect_space_if_string(current_state));
	else
	{
		free_command(current);
		return (EXIT);
	}
}

t_ast_node	*extract_command(t_list **list, t_state_context *context)
{
	t_ast_node	*current;
	t_token		*cur_tok;
	int			current_state;

	cur_tok = (t_token *)(*list)->content;
	current = (t_ast_node *)ft_calloc(sizeof(t_ast_node), 1);
	context->current = current;
	current_state = initialize_command_state(cur_tok, &current);
	while (current_state != EXIT)
	{
		cur_tok = (t_token *)(*list)->content;
		*list = (*list)->next;
		current_state = process_state(current_state, list,
				cur_tok, context);
	}
	return (context->current);
}
