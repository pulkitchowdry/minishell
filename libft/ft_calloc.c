/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 11:28:55 by pchowdry          #+#    #+#             */
/*   Updated: 2025/06/14 15:38:23 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	total_size;

	if (nmemb == 0 || size == 0)
		return (malloc(1));
	if (nmemb > (size_t)-1 / size)
		return (NULL);
	total_size = nmemb * size;
	ptr = malloc(total_size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, total_size);
	return (ptr);
}

// #include <stdio.h>
// #include <limits.h>
// #include <stdlib.h>

// int	main(void)
// {
// 	size_t	n = 0;
// 	size_t	i = 0;
// 	int	*arr = ft_calloc(INT_MIN, INT_MIN);
// 	if (!arr)
// 	{
// 		printf("Allocation failed\n");
// 		return (1);		
// 	}
// 	while (i < n)
// 	{
// 		if (arr[i] != 0)
// 		{
// 			printf("Memory not 0 at %zu\n", i);
// 			free(arr);
// 			return (1);
// 		}
// 		i++;
// 	}
// 	printf("Memory initialized OK\n");
// 	free(arr);
// 	return (0);
// }