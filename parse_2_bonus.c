/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_2_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:11:12 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/31 15:21:59 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell_bonus.h"

#include <stdlib.h>

extern int	g_ret_code;

char	is_variable_match(char *string, char *variable)
{
	char	*variable_key;
	char	match;

	variable_key = get_variable_key(string);
	match = ft_strncmp(variable_key, variable,
			ft_strlen(variable_key) + 1) == 0;
	free(variable_key);
	return (match);
}

char	*get_value(t_variable_context *context, char *variable)
{
	int		seek;

	seek = 0;
	while (context->environment_variables[seek])
	{
		if (is_variable_match(context->environment_variables[seek], variable))
			return (get_variable_value(context->environment_variables[seek]));
		seek++;
	}
	seek = 0;
	while (context->local_variables && context->local_variables[seek])
	{
		if (is_variable_match(context->local_variables[seek], variable))
			return (get_variable_value(context->local_variables[seek]));
		seek++;
	}
	return (get_special_variable(variable));
}

char	*free_and_return_original_string(char *string, char *head, char *tail)
{
	free(head);
	free(tail);
	return (string);
}

char	*substitute_variable(char *string, char *start,
	char *end, t_variable_context *context)
{
	char	*result;
	char	*head;
	char	*variable;
	char	*value;
	char	*tail;

	head = ft_substr(string, 0, start - string - 1);
	tail = ft_substr(string, end - string, ft_strlen(string) - (end - string));
	variable = extract_variable_name(string, start, end);
	if (variable == 0)
		return (free_and_return_original_string(string, head, tail));
	value = get_value(context, variable);
	free(variable);
	result = (char *)ft_calloc(sizeof(char), ft_strlen(head) + ft_strlen(value)
			+ ft_strlen(tail) + 1);
	ft_strlcat(result, head, ft_strlen(head) + 1);
	ft_strlcat(result, value, ft_strlen(head) + ft_strlen(value) + 1);
	ft_strlcat(result, tail, ft_strlen(head)
		+ ft_strlen(value) + ft_strlen(tail) + 1);
	free(head);
	free(value);
	free(tail);
	free(string);
	return (result);
}

char	*process_double_quote(char *string, t_variable_context *context)
{
	char	*end;
	char	*start;
	char	curly_brackets;
	char	*result;

	result = ft_strtrim(string, "\"");
	start = ft_strchr(result, '$');
	while (start)
	{
		end = start + 1;
		curly_brackets = *end == '{';
		end += curly_brackets;
		while ((ft_isalnum(*end) || *end == '_'
				|| (curly_brackets && *end == '}') || *end == '?') && *end)
		{
			end++;
			if (curly_brackets && *end == '}')
				break ;
		}
		result = substitute_variable(result, start + 1,
				end + (curly_brackets && *end == '}'), context);
		start = ft_strchr(result, '$');
	}
	return (result);
}
