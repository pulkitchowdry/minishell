/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:37:50 by pchowdry          #+#    #+#             */
/*   Updated: 2025/06/14 18:18:06 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	i = 0;
	ptr = (unsigned char *) s;
	while (i < n)
	{
		if (ptr[i] == (unsigned char) c)
			return (&ptr[i]);
		i++;
	}
	return (0);
}

// #include <stdio.h>
// #include <string.h>

// int	main(void)
// {
// 	char	*src = "hello";
// 	int	c = 'o';
// 	int	n = 14;
// 	char	*sys = (char *) memchr(src,c,n);
// 	char	*my = (char *) ft_memchr(src,c,n);

// 	printf("Sys: %s\n", sys);
// 	printf("My: %s\n", my);
// }