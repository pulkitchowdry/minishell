/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 12:55:19 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/26 18:30:07 by chikoh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

#include "minishell.h"
#include "libft/libft.h"

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

char	test_for_no_ends(char **list_of_strings, char *filename)
{
	size_t	index;
	size_t	str_index;

	if (filename[0] == '.')
		return (0);
	else if (ft_size(list_of_strings) == 0 && filename[0] != '.')
		return (1);
	index = 0;
	str_index = 0;
	while (index < ft_size(list_of_strings) && str_index < ft_strlen(filename))
	{
		index += ft_strncmp(filename + str_index, list_of_strings[index],
				ft_strlen(list_of_strings[index])) == 0;
		str_index++;
	}
	return (index == ft_size(list_of_strings));
}

char	is_match(char head_flag, char tail_flag,
		char **list_of_strings, char *filename)
{
	if (head_flag && tail_flag)
		return (test_for_both_ends(list_of_strings, filename));
	else if (!head_flag && tail_flag)
		return (test_for_tail(list_of_strings, filename));
	else if (head_flag && !tail_flag)
		return (test_for_head(list_of_strings, filename));
	else
		return (test_for_no_ends(list_of_strings, filename));
}

void	filter_on_wildcard(char head_flag, char tail_flag,
		char **list_of_strings, char **filenames)
{
	int	filter_idx;
	int	max_files;

	filter_idx = 0;
	max_files = ft_size(filenames);
	while (filter_idx < max_files)
	{
		if (!is_match(head_flag, tail_flag,
				list_of_strings, filenames[filter_idx]))
		{
			free(filenames[filter_idx]);
			filenames[filter_idx] = 0;
		}
		filter_idx++;
	}
	compact_filenames(max_files, filenames);
}

t_list	*create_list_of_matches(char **filenames)
{
	int		seek;
	t_list	*result;

	result = 0;
	seek = 0;
	while (filenames[seek])
	{
		ft_lstadd_back(&result, ft_lstnew(ft_strdup(filenames[seek])));
		seek++;
	}
	return (result);
}

t_list	*find_matches(char head_flag, char tail_flag, char **list_of_strings)
{
	DIR				*cur_dir;
	struct dirent	*file;
	int				dir_num;
	char			**filenames;
	t_list			*result;

	cur_dir = opendir("./");
	if (cur_dir == 0)
		return (0);
	file = readdir(cur_dir);
	dir_num = 1;
	while (file)
	{
		file = readdir(cur_dir);
		dir_num++;
	}
	closedir(cur_dir);
	filenames = get_sorted_filenames(dir_num);
	filter_on_wildcard(head_flag, tail_flag, list_of_strings, filenames);
	result = create_list_of_matches(filenames);
	free_string_array(filenames);
	return (result);
}

t_list	*find_match_string(char *string_with_wildcard)
{
	char	**list_of_strings;
	t_list	*list_of_matches;

	if (string_with_wildcard == 0)
		return (0);
	list_of_strings = ft_split(string_with_wildcard, '*');
	if (list_of_strings == 0)
		return (0);
	list_of_matches = 0;
	if (ft_size(list_of_strings) != 0)
		list_of_matches = find_matches(ft_strncmp(string_with_wildcard,
					list_of_strings[0],
					ft_strlen(list_of_strings[0])) == 0,
				ft_strncmp(string_with_wildcard
					+ ft_strlen(string_with_wildcard)
					- ft_strlen(list_of_strings[ft_size(list_of_strings)
						- 1]), list_of_strings[ft_size(list_of_strings) - 1],
					ft_strlen(list_of_strings[ft_size(list_of_strings)
						- 1])) == 0, list_of_strings);
	else if (ft_strncmp("*", string_with_wildcard, 1) == 0)
		list_of_matches = find_matches(0, 0, list_of_strings);
	free_string_array(list_of_strings);
	return (list_of_matches);
}
