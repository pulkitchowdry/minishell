/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:16:59 by pchowdry          #+#    #+#             */
/*   Updated: 2025/06/14 18:23:39 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 97 && c <= 122)
		c = c - 32;
	return (c);
}

/* #include <ctype.h>
#include <stdio.h>
int	main(void)
{
	printf("Value is: %c\n",ft_toupper('c'));
	printf("Value is: %c\n",toupper('c'));
	
} */