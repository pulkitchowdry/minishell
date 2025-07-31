/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 18:54:37 by pchowdry          #+#    #+#             */
/*   Updated: 2025/06/14 18:21:21 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (src[i] != '\0' && i < size - 1)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (ft_strlen(src));
}

// #include <stdio.h>
// #include <string.h>
// int	main(void)
// {
// 	// char	src_sys[] = "hello";
// 	// char	dest_sys[] = "blah";
// 	char	src_my[] = "hello";
// 	char	dest_my[] = "blah";

// 	// printf("Sys func value: %d\n", strlcpy(dest_sys, src_sys, 4));
// 	printf("My func value: %d\n", ft_strlcpy(dest_my, src_my, 2));
// 	printf("Dest value: %s", dest_my);
// }