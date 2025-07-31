/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:26:49 by pchowdry          #+#    #+#             */
/*   Updated: 2025/07/04 16:29:02 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_extract_num(const char *nptr, int i, int num)
{
	while (nptr[i] >= 48 && nptr[i] <= 57)
	{
		num = num * 10 + (nptr[i] - '0');
		i++;
	}
	return (num);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	num;
	int	neg;
	int	sign;

	i = 0;
	num = 0;
	neg = 0;
	sign = 0;
	while (((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32))
		i++;
	while (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			neg++;
		i++;
		sign++;
	}
	if (sign > 1)
		return (0);
	num = ft_extract_num (nptr, i, num);
	if (neg % 2 != 0)
		num = -num;
	return (num);
}

// #include <stdio.h>
// #include <stdlib.h>
// int	main(void)
// {
// 	char	*str = "      	-2147483648abc1023";
// 	printf("Sys: %d\n", atoi(str));
// 	printf("MY: %d\n", ft_atoi(str));
// 	return (0);
// }