/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:18:03 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/29 23:20:56 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

#include "minishell.h"
#include "libft/libft.h"

extern int	g_ret_code;

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
