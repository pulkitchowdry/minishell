/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:50:22 by pchowdry          #+#    #+#             */
/*   Updated: 2025/06/14 18:05:19 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

// int main(void)
// {
//     int c = 'a';

//         printf("Sys func output %d\n", isdigit(c));
//         printf("My func output %d\n", ft_isdigit(c));

//     return(0);
// }