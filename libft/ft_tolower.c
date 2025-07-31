/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:23:03 by pchowdry          #+#    #+#             */
/*   Updated: 2025/06/14 18:23:27 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 65 && c <= 90)
		c = c + 32;
	return (c);
}

// #include <ctype.h>
// #include <stdio.h>
// int	main(void)
// {
// 	printf("Value is: %c\n",ft_tolower('D'));
// 	printf("Value is: %c\n",tolower('D'));

// }