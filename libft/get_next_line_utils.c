/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chikoh <chikoh@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:42:36 by chikoh            #+#    #+#             */
/*   Updated: 2025/05/23 18:29:25 by chikoh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "get_next_line.h"

char	*duplicate_string(char *str)
{
	size_t	index;
	char	*result;

	result = (char *)malloc(sizeof(char) * (len_str(str) + 1));
	if (result == 0)
		return (0);
	index = 0;
	while (str[index])
	{
		result[index] = str[index];
		index++;
	}
	result[index] = 0;
	return (result);
}

static char	*free_and_ret_null(char *str)
{
	free(str);
	return (0);
}

char	*finalize_string(char *result, t_file_header *header,
	size_t start_idx)
{
	char	*substring;
	size_t	idx;
	char	contains_newline;

	if (len_str(result) == 0 && header->buffer[start_idx] == 0)
		return (free_and_ret_null(result));
	contains_newline = header->buffer[header->seek] == '\n';
	substring = (char *)malloc(sizeof(char)
			* (header->seek - start_idx + contains_newline + 1));
	if (substring == 0)
		return (free_and_ret_null(result));
	idx = 0;
	while (idx < (header->seek - start_idx + contains_newline))
	{
		substring[idx] = header->buffer[idx + start_idx];
		idx++;
	}
	substring[idx] = 0;
	result = concat_string(result, substring);
	header->seek++;
	return (result);
}

static char	*ft_store_string(char *buffer, size_t size)
{
	char	*temp;
	size_t	index;

	temp = (char *)malloc(sizeof(char) *(size + 1));
	if (temp == 0)
		return (0);
	index = 0;
	while (index < size)
	{
		temp[index] = buffer[index];
		index++;
	}
	temp[index] = 0;
	return (temp);
}

char	*append_result_buffer(t_file_header *header, char *result,
		int fd, size_t *starting_index)
{
	char	*temp;

	temp = ft_store_string(&(header->buffer[*starting_index]),
			BUFFER_SIZE - *starting_index);
	if (temp == 0)
		return (0);
	read_block(fd, header);
	*starting_index = 0;
	result = concat_string(result, temp);
	return (result);
}
