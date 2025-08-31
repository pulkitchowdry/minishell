/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_10.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chikoh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 21:21:01 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/31 14:13:52 by chikoh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	substitute_node_for_double_quote(t_list **prev,
		t_list **command, t_variable_context *context)
{
	char	*result;

	result = process_double_quote((char *)(*command)->content, context);
	free((*command)->content);
	(*command)->content = result;
	*prev = *command;
	*command = (*command)->next;
}

void	substitute_node_for_single_quote(t_list **prev,
		t_list **command)
{
	char	*result;

	result = process_single_quote((char *)(*command)->content);
	free((*command)->content);
	(*command)->content = result;
	*prev = *command;
	*command = (*command)->next;
}

void	substitute_node_for_variable(t_list **result, t_list **prev,
		t_list **command, t_variable_context *context)
{
	(*command)->content = process_variable((char *)(*command)->content, context);
	substitute_node_with_list(result, prev, command);
}

