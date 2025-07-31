/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:07:35 by pchowdry          #+#    #+#             */
/*   Updated: 2025/06/14 18:19:10 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*temp_src;
	unsigned char	*temp_dest;
	size_t			i;

	temp_src = (unsigned char *) src;
	temp_dest = (unsigned char *) dest;
	if (temp_dest < temp_src)
	{
		i = 0;
		while (i < n)
		{
			temp_dest[i] = temp_src[i];
			i++;
		}
	}
	else
	{
		i = n;
		while (i > 0)
		{
			i--;
			temp_dest[i] = temp_src[i];
		}
	}
	return (dest);
}

// #include <string.h>
// #include <stdio.h>
// #include <string.h>
// #include <stdio.h>

// int	main(void)
// {
// 	unsigned char	str[] = "hello";
// 	// unsigned char	dest[] = "world";

// 	printf("Value of dest: %s\n", str);
// 	printf("Sys func: %s\n", (unsigned char *) memmove(str, str+2, 2));
// 	printf("My func: %s", (unsigned char *) ft_memmove(str, str+2, 2));
// 	return (0);
// }

// int	main(void)
// {
// 	unsigned char	src[15];
// 	unsigned char	dest[15];
// 	int	i = 0;

// 	while (i < 9)
// 	{
// 		src[i] = 'a';
// 		dest[i] = 'b';
// 		i++;
// 	}
// 	src[i] = '\0';
// 	dest[i] = '\0';
// 	printf("Src before sys func: %s\n", src);
// 	printf("Dest before sys func: %s\n\n", dest);
// 	printf("Src before my func: %s\n", src);
// 	printf("Dest before my func: %s\n\n", dest);

// 	memmove(dest + 3, src, 4);
// 	ft_memmove(dest + 3, src, 4);

// 	printf("Src after sys func: %s\n", src);
// 	printf("Dest after sys func: %s\n\n", dest);
// 	printf("Src after my func: %s\n", src);
// 	printf("Dest after my func: %s\n", dest);
// }