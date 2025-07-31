/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:17:09 by pchowdry          #+#    #+#             */
/*   Updated: 2025/06/15 15:49:10 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (little[i] == '\0')
		return ((char *) big);
	while (big[i] && i < len)
	{
		j = 0;
		while (little[j] && big[i + j] && i + j < len
			&& big[i + j] == little[j])
			j++;
		if (little[j] == '\0')
			return ((char *) &big[i]);
		i++;
	}
	return (0);
}

// #include <string.h>
// #include <stdio.h>
// int	main(void)
// {
// 	char	*big = "abcde";
// 	char	*little= "cd";

// 	printf("My: %s", ft_strnstr(big, little, 5));
// 	return (0);
// }