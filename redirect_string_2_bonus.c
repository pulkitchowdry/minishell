/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_string_2_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:14:34 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/31 15:22:40 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

extern int	g_ret_code;

int	determine_next_redirect_space_if_space(int current_state)
{
	if (current_state == ASSIGN_REDIRECT_SPACE)
		return (ASSIGN_REDIRECT_SPACE);
	else if (current_state == COMMAND_REDIRECT_SPACE)
		return (COMMAND_REDIRECT_SPACE);
	return (EXIT);
}

int	determine_next_redirect_space_if_string(int current_state)
{
	if (current_state == ASSIGN_REDIRECT_SPACE)
		return (ASSIGN_REDIRECT_STRING);
	else if (current_state == COMMAND_REDIRECT_SPACE)
		return (COMMAND_REDIRECT_STRING);
	return (EXIT);
}
