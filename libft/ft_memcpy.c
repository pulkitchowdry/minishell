/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:06:30 by pchowdry          #+#    #+#             */
/*   Updated: 2025/06/14 18:18:49 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*temp_src;
	unsigned char	*temp_dest;
	size_t			i;

	temp_src = (unsigned char *) src;
	temp_dest = (unsigned char *) dest;
	i = 0;
	while (i < n)
	{
		temp_dest[i] = temp_src[i];
		i++;
	}
	return (dest);
}

// #include <string.h>
// #include <stdio.h>

// int	main(void)
// {
// 	unsigned char	str[] = "hello";
// 	// unsigned char	dest[] = "world";

// 	printf("Value of dest: %s\n", str);
// 	printf("Sys func: %s\n", (unsigned char *) memcpy(str+2, str, 1));
// 	printf("My func: %s", (unsigned char *) ft_memcpy(str+2, str, 1));
// 	return (0);
// }