/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 12:55:19 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/29 23:18:30 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

#include "minishell.h"
#include "libft/libft.h"

extern int	g_ret_code;

size_t	ft_size(char **string_array)
{
	size_t	count;

	if (string_array == 0)
		return (0);
	count = 0;
	while (*string_array)
	{
		count++;
		string_array++;
	}
	return (count);
}

char	**append_string_array(char **string_array, char *string)
{
	int		seek;
	char	**result;

	result = (char **)ft_calloc(sizeof(char *), ft_size(string_array) + 1);
	seek = 0;
	while (string_array[seek])
	{
		result[seek] = string_array[seek];
		seek++;
	}
	result[seek] = string;
	free_string_array(string_array);
	return (result);
}

void	free_string_array(char **string_array)
{
	char	**array_start;

	if (string_array == 0)
		return ;
	array_start = string_array;
	while (*string_array)
	{
		free(*string_array);
		string_array++;
	}
	free(array_start);
}

size_t	max_string_len(char **string)
{
	size_t	count;

	count = 0;
	while (*string)
	{
		if (count < ft_strlen(*string))
			count = ft_strlen(*string);
		string++;
	}
	return (count);
}

void	sort_string_ascii(char **string)
{
	size_t		i;
	size_t		j;
	char		*temp;

	i = 0;
	while (i < ft_size(string))
	{
		j = i + 1;
		while (j < ft_size(string))
		{
			if (ft_strncmp(string[i], string[j],
					max_string_len(string)) > 0)
			{
				temp = string[j];
				string[j] = string[i];
				string[i] = temp;
			}
			j++;
		}
		i++;
	}
}
