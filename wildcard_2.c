/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:17:22 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/29 23:21:01 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

#include "minishell.h"
#include "libft/libft.h"

extern int	g_ret_code;

char	**get_sorted_filenames(int dir_num)
{
	DIR				*cur_dir;
	struct dirent	*file;
	char			**filenames;
	int				index;

	cur_dir = opendir("./");
	if (cur_dir == 0)
		return (0);
	filenames = (char **)ft_calloc(sizeof(char *), dir_num + 1);
	file = readdir(cur_dir);
	index = 0;
	while (file)
	{
		filenames[index] = ft_strdup(file->d_name);
		file = readdir(cur_dir);
		index++;
	}
	closedir(cur_dir);
	sort_string_ascii(filenames);
	return (filenames);
}

void	compact_filenames(int max_files, char **filenames)
{
	int	seek;
	int	compact;

	seek = 0;
	while (seek < max_files)
	{
		if (filenames[seek] == 0)
		{
			compact = seek;
			while (filenames[compact] == 0 && compact < max_files)
				compact++;
			if (compact < max_files)
			{
				filenames[seek] = filenames[compact];
				filenames[compact] = 0;
			}
		}
		seek++;
	}
}

char	test_for_both_ends(char **list_of_strings, char *filename)
{
	size_t	index;
	size_t	str_index;

	if ((ft_strncmp(filename, list_of_strings[0],
				ft_strlen(list_of_strings[0])) != 0)
		|| ft_strncmp(filename + ft_strlen(filename)
			- ft_strlen(list_of_strings[ft_size(list_of_strings) - 1]),
			list_of_strings[ft_size(list_of_strings) - 1],
			ft_strlen(list_of_strings[ft_size(list_of_strings) - 1])) != 0
		|| ft_strlen(filename) == 1)
		return (0);
	index = 1;
	str_index = ft_strlen(list_of_strings[0]);
	while (index < (ft_size(list_of_strings) - 1)
		&& str_index < (ft_strlen(filename)
			- ft_strlen(list_of_strings[ft_size(list_of_strings) - 1])))
	{
		index += ft_strncmp(filename + str_index, list_of_strings[index],
				ft_strlen(list_of_strings[index])) == 0;
		str_index++;
	}
	return (index == (ft_size(list_of_strings) - 1));
}

char	test_for_tail(char **list_of_strings, char *filename)
{
	size_t	index;
	size_t	str_index;

	if (ft_strncmp(filename + ft_strlen(filename)
			- ft_strlen(list_of_strings[ft_size(list_of_strings) - 1]),
			list_of_strings[ft_size(list_of_strings) - 1],
			ft_strlen(list_of_strings[ft_size(list_of_strings) - 1])) != 0)
		return (0);
	index = 0;
	str_index = 0;
	while (index < (ft_size(list_of_strings) - 1)
		&& str_index < ft_strlen(filename)
		- ft_strlen(list_of_strings[ft_size(list_of_strings) - 1]))
	{
		index += ft_strncmp(filename + str_index, list_of_strings[index],
				ft_strlen(list_of_strings[index])) == 0;
		str_index++;
	}
	return (index == (ft_size(list_of_strings) - 1));
}

char	test_for_head(char **list_of_strings, char *filename)
{
	size_t	index;
	size_t	str_index;

	if (ft_strncmp(filename, list_of_strings[0],
			ft_strlen(list_of_strings[0])) != 0)
		return (0);
	index = 1;
	str_index = ft_strlen(list_of_strings[0]);
	while (index < ft_size(list_of_strings) && str_index < ft_strlen(filename))
	{
		index += ft_strncmp(filename + str_index, list_of_strings[index],
				ft_strlen(list_of_strings[index])) == 0;
		str_index++;
	}
	return (index == ft_size(list_of_strings));
}
