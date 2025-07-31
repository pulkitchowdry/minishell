/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:19:03 by pchowdry          #+#    #+#             */
/*   Updated: 2025/07/13 17:50:55 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*combine;
	size_t	i;
	size_t	s1_len;
	size_t	s2_len;

	i = 0;
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	combine = malloc((s1_len + s2_len + 1) * sizeof(char));
	if (!combine)
		return (NULL);
	while (i < s1_len)
	{
		combine[i] = s1[i];
		i++;
	}
	while (i < s1_len + s2_len)
	{
		combine[i] = s2[i - s1_len];
		i++;
	}
	combine[i] = '\0';
	return (combine);
}

// #include <stdio.h>

// int	main(void)
// {
// 	char	*res = ft_strjoin("hello", " world");
// 	printf("My: %s", res);
// 	free(res);
// 	return (0);
// }