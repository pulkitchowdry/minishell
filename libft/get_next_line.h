/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chikoh <chikoh@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:42:11 by chikoh            #+#    #+#             */
/*   Updated: 2025/05/23 18:29:34 by chikoh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 50
# endif

# include <unistd.h>
# include <stddef.h>

typedef struct s_file_header
{
	char	buffer[BUFFER_SIZE];
	size_t	seek;
	int		fd;
	ssize_t	bytes_read;
}		t_file_header;

char	*get_next_line(int fd);
char	*duplicate_string(char *str);
size_t	len_str(char *str);
char	*finalize_string(char *result, t_file_header *header,
			size_t start_idx);
char	*concat_string(char *str1, char *str2);
void	read_block(int fd, t_file_header *header);
char	*append_result_buffer(t_file_header *header, char *result,
			int fd, size_t *starting_index);
#endif
