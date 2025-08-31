/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_string_next_state.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 18:43:25 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/29 23:14:17 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_ret_code;

int	determine_next_redirect_string_if_space(int current_state)
{
	if (current_state == ASSIGN_REDIRECT_STRING)
		return (ASSIGN_SPACE);
	else if (current_state == COMMAND_REDIRECT_STRING)
		return (COMMAND_SPACE);
	return (EXIT);
}

int	determine_next_redirect_string_if_command(int current_state)
{
	if (current_state == ASSIGN_REDIRECT_STRING)
		return (ASSIGN_REDIRECT_STRING);
	else if (current_state == COMMAND_REDIRECT_STRING)
		return (COMMAND_REDIRECT_STRING);
	return (EXIT);
}

int	determine_next_redirect_string_if_redirect(int current_state)
{
	if (current_state == ASSIGN_REDIRECT_STRING
		|| current_state == ASSIGN_REDIRECT)
		return (ASSIGN_REDIRECT);
	else if (current_state == COMMAND_REDIRECT_STRING
		|| current_state == COMMAND_REDIRECT)
		return (COMMAND_REDIRECT);
	return (EXIT);
}

int	determine_next_redirect_if_command_string(int current_state)
{
	if (current_state == ASSIGN_REDIRECT)
		return (ASSIGN_REDIRECT_STRING);
	else if (current_state == COMMAND_REDIRECT)
		return (COMMAND_REDIRECT_STRING);
	return (EXIT);
}

int	determine_next_redirect_if_space(int current_state)
{
	if (current_state == ASSIGN_REDIRECT)
		return (ASSIGN_REDIRECT_SPACE);
	else if (current_state == COMMAND_REDIRECT)
		return (COMMAND_REDIRECT_SPACE);
	return (EXIT);
}
