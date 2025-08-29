/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_new_node_ext_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:10:23 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/29 23:21:25 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

extern int	g_ret_code;

char	is_command_string(int type)
{
	return (type == DOUBLE_QUOTE_STRING
		|| type == SINGLE_QUOTE_STRING
		|| type == STRING || type == ASSIGNMENT
		|| type == VARIABLE);
}

char	is_pipe_or_logical(int type)
{
	return (type == PIPE
		|| type == LOGICAL_AND
		|| type == LOGICAL_OR
		|| type == CLOSE_BRACKET);
}

char	is_redirect(int type)
{
	return (type == REDIRECT_INPUT || type == REDIRECT_OUTPUT
		|| type == REDIRECT_APPEND || type == HERE_DOC);
}
