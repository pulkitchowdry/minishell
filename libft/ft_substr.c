/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:03:31 by pchowdry          #+#    #+#             */
/*   Updated: 2025/06/14 18:23:07 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	i;
	size_t	total;
	size_t	size;

	i = 0;
	total = (size_t)start + len;
	if (start > ft_strlen(s))
		size = 0;
	else if (total > ft_strlen(s))
		size = ft_strlen(s) - start;
	else
		size = len;
	sub = malloc((size + 1) * sizeof(char));
	if (!sub)
		return (NULL);
	while (s[i] != '\0' && start < ft_strlen(s) && i < len)
	{
		sub[i] = s[start];
		i++;
		start++;
	}
	sub[i] = '\0';
	return (sub);
}

// #include <stdio.h>
// int	main(void)
// {
// 	char	*res = ft_substr("0123456789", 1, 9);
// 	printf("MY: %s", res);
// 	free(res);
// 	return (0);
// }