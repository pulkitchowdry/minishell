/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:01:18 by pchowdry          #+#    #+#             */
/*   Updated: 2025/06/14 18:18:34 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*s1_ptr;
	unsigned char	*s2_ptr;

	i = 0;
	s1_ptr = (unsigned char *) s1;
	s2_ptr = (unsigned char *) s2;
	while (i < n)
	{
		if (s1_ptr[i] != s2_ptr[i])
			return (s1_ptr[i] - s2_ptr[i]);
		i++;
	}
	return (0);
}

// #include <string.h>
// #include <stdio.h>

// int	main(void)
// {
// 	char	s1[] = {-128, 0, 127, 0};
// 	char	s2[] = {-128, 0, 127, 0};
// 	int	n = 4;

// 	printf("Sys: %d\n", memcmp(s1, s2, n));
// 	printf("My: %d\n", ft_memcmp(s1, s2, n));
// }