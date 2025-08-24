/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chikoh <chikoh@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:41:47 by chikoh            #+#    #+#             */
/*   Updated: 2025/05/23 21:50:59 by chikoh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>

#include "get_next_line.h"

void	read_block(int fd, t_file_header *header)
{
	header->bytes_read = read(fd, header->buffer, BUFFER_SIZE);
	if (header->bytes_read < BUFFER_SIZE && header->bytes_read >= 0)
		header->buffer[header->bytes_read] = 0;
	header->seek = 0;
	header->fd = fd;
}

size_t	len_str(char *str)
{
	size_t	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

static size_t	append_string(char *dst, char *src, size_t offset)
{
	size_t	str_len;

	str_len = 0;
	while (src[str_len])
	{
		dst[offset + str_len] = src[str_len];
		str_len++;
	}
	dst[offset + str_len] = 0;
	return (str_len);
}

char	*concat_string(char *str1, char *str2)
{
	char	*result;

	result = (char *)malloc(sizeof(char) * (len_str(str1) + len_str(str2) + 1));
	if (result == 0)
	{
		free(str1);
		free(str2);
		return (0);
	}
	result[0] = 0;
	append_string(result, str2, append_string(result, str1, 0));
	free(str1);
	free(str2);
	return (result);
}

char	*get_next_line(int fd)
{
	static t_file_header	header = {.seek = -1, .fd = -1,
		.bytes_read = BUFFER_SIZE};
	char					*result;
	size_t					starting_index;

	if (fd < 0)
		return (0);
	if ((ssize_t)header.seek >= header.bytes_read || header.fd != fd)
		read_block(fd, &header);
	if (header.bytes_read < 0)
		return (0);
	result = duplicate_string("");
	if (result == 0)
		return (0);
	starting_index = ((ssize_t)header.seek < header.bytes_read) * header.seek;
	while (header.buffer[header.seek] != '\n' && header.buffer[header.seek])
	{
		if (header.seek + 1 >= BUFFER_SIZE)
			result = append_result_buffer(&header, result, fd, &starting_index);
		else
			header.seek++;
	}
	return (finalize_string(result, &header, starting_index));
}
