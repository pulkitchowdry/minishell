/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 20:17:48 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/31 15:22:12 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell_bonus.h"

#include <stdlib.h>

extern int	g_ret_code;

char	*extract_variable_name(char *string, char *start, char *end)
{
	if (*start == '{')
	{
		start++;
		end--;
		end += *end != '}';
	}
	return (ft_substr(string, start - string, end - start));
}

char	*get_variable_key(char *string)
{
	char	*equal_sign;

	equal_sign = ft_strchr(string, '=');
	if (equal_sign == 0)
		return (ft_strdup(""));
	return (ft_substr(string, 0, equal_sign - string));
}

char	**set_variable_string(char **variable_list, char *string)
{
	char	*key;
	char	*new_key;
	int		seek;
	char	match;

	seek = 0;
	while (variable_list[seek])
	{
		key = get_variable_key(variable_list[seek]);
		new_key = get_variable_key(string);
		match = ft_strncmp(key, new_key, ft_strlen(new_key) + 1) == 0;
		free(key);
		free(new_key);
		if (match)
		{
			free(variable_list[seek]);
			variable_list[seek] = string;
			return (variable_list);
		}
	}
	return (append_string_array(variable_list, string));
}

char	*get_variable_value(char *string)
{
	char	*equal_sign;

	equal_sign = ft_strchr(string, '=');
	if (equal_sign == 0)
		return (ft_strdup(""));
	return (ft_substr(string, equal_sign - string + 1,
			ft_strlen(equal_sign + 1)));
}

char	*get_special_variable(char *variable)
{
	if (ft_strncmp(variable, "?", 2) == 0)
		return (ft_itoa(g_ret_code));
	return (ft_strdup(""));
}
