/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 11:06:16 by pchowdry          #+#    #+#             */
/*   Updated: 2025/06/14 18:06:21 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_n_len(int n)
{
	long	temp;
	int		len;

	len = 0;
	temp = n;
	if (temp == 0)
		len++;
	if (temp < 0)
	{
		len++;
		temp = -temp;
	}
	while (temp > 0)
	{
		len++;
		temp = temp / 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*num;
	int		i;
	long	nb;

	i = ft_n_len(n);
	nb = n;
	num = malloc((i + 1) * sizeof(char));
	if (!num)
		return (NULL);
	if (nb < 0)
	{
		num[0] = '-';
		nb = -nb;
	}
	if (nb == 0)
		num[0] = '0';
	num[i] = '\0';
	while (nb > 0 && i > 0)
	{
		i--;
		num[i] = (nb % 10) + '0';
		nb = nb / 10;
	}
	return (num);
}

// #include <stdio.h>
// int	main(void)
// {
// 	char	*res = ft_itoa(1);
// 	printf("My: %s", res);
// 	return (0);
// }