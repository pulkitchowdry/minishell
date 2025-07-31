/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 19:16:24 by pchowdry          #+#    #+#             */
/*   Updated: 2025/07/13 18:22:14 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	src_len;
	size_t	dest_len;
	size_t	i;

	src_len = ft_strlen(src);
	dest_len = 0;
	while (dest[dest_len] && dest_len < size)
		dest_len++;
	i = 0;
	if (dest_len == size)
		return (dest_len + src_len);
	while (src[i] != '\0' && i < size - dest_len - 1)
	{
		dest[i + dest_len] = src[i];
		i++;
	}
	if (i < size - dest_len)
		dest[i + dest_len] = '\0';
	return (dest_len + src_len);
}

// #include <stdio.h>
// #include <string.h>
// int	main(void)
// {
// 	// char	src_sys[] = "hello";
// 	// char	dest_sys[] = "blah";
// 	char	src_my[] = "AAAAAAAAA";
// 	char	dest_my[] = "B";

// 	// printf("Sys func value: %d\n", strlcpy(dest_sys, src_sys, 4));
// 	printf("My func value: %d\n", ft_strlcat(dest_my, src_my, 3));
// 	printf("Dest value: %s", dest_my);
// }