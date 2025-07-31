/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:32:32 by pchowdry          #+#    #+#             */
/*   Updated: 2025/06/14 18:20:26 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// void to_upper(unsigned int i, char *c)
// {
//     (void)i; // if index not needed
//     *c = ft_toupper(*c);
// }
void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	int	i;

	i = 0;
	if (!s || !f)
		return ;
	while (s[i] != '\0')
	{
		f(i, &s[i]);
		i++;
	}
}

// #include <stdio.h>
// int	main(void)
// {
// 	char	res[] = "hello";
// 	ft_striteri(res, to_upper);
// 	printf("%s\n", res);
// 	return (0);
// }