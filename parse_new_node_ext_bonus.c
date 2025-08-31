/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_new_node_ext_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:09:41 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/31 15:22:21 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell_bonus.h"

extern int	g_ret_code;

char	is_command_redirect_space(int current_state)
{
	return (current_state == COMMAND_REDIRECT_SPACE
		|| current_state == ASSIGN_REDIRECT_SPACE);
}

char	is_command_redirect_string(int current_state)
{
	return (current_state == COMMAND_REDIRECT_STRING
		|| current_state == ASSIGN_REDIRECT_STRING);
}

char	is_command_redirect(int current_state)
{
	return (current_state == COMMAND_REDIRECT
		|| current_state == ASSIGN_REDIRECT);
}

int	process_state(int current_state, t_list **list,
		t_token *cur_tok, t_state_context *state_context)
{
	if (current_state == ASSIGN_STRING_VAL)
		return (process_assign_string_val(list, cur_tok,
				&state_context->current, state_context->context));
	else if (current_state == ASSIGN_SPACE)
		return (process_assign_space(list, &state_context->current));
	else if (current_state == ASSIGN_OP)
		return (process_assign_op(list, &state_context->current));
	else if (current_state == INITIAL_COMMAND_STRING)
		return (process_initial_command_string(list, cur_tok,
				&state_context->current, state_context->context));
	else if (current_state == COMMAND_SPACE)
		return (process_command_space(list, &state_context->current));
	else if (is_command_redirect(current_state))
		return (process_redirect(list, cur_tok,
				&state_context->current, current_state));
	else if (is_command_redirect_space(current_state))
		return (process_redirect_space(list,
				&state_context->current, current_state));
	else if (is_command_redirect_string(current_state))
		return (process_redirect_string(list,
				cur_tok, current_state, state_context));
	else if (current_state == COMMAND_QUOTES || current_state == COMMAND_STRING)
		return (process_command_string(list, cur_tok,
				&state_context->current, state_context->context));
	return (EXIT);
}

void	append_string_to_current_node(t_token *cur_tok,
		t_list *list, t_variable_context *context)
{
	char	*string;
	t_list	*last_node;

	(void)context;
	last_node = ft_lstlast(list);
	string = (char *)last_node->content;
	last_node->content = ft_strjoin(string, cur_tok->string);
	free(string);
}
