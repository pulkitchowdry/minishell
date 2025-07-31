/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:06:10 by pchowdry          #+#    #+#             */
/*   Updated: 2025/06/15 15:47:50 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;

	i = ft_strlen(s);
	if (c == '\0')
		return ((char *) &s[i]);
	while (i > 0)
	{
		i--;
		if (s[i] == (char) c)
			return ((char *) &s[i]);
	}
	return (0);
}

// #include <string.h>
// #include <stdio.h>

// // int	main(void)
// // {
// // 	char	*s="easdasdaksdlkasdadkasd";
// // 	int	c = 'j';
// // 	printf("Sys: Last value of %c is in position %s\n", c, strrchr(s, c));
// // 	printf("My func: Last val of %c is in pos %s\n", c, ft_strrchr(s, c));
// // 	return (0);
// // }