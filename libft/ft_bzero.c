/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:09:29 by pchowdry          #+#    #+#             */
/*   Updated: 2025/06/14 17:57:54 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *str, size_t n)
{
	unsigned char	*ptr;
	size_t			i;

	i = 0;
	ptr = (unsigned char *) str;
	while (i < n)
	{
		ptr[i] = '\0';
		i++;
	}
}

// #include <strings.h>
// #include <stdio.h>

// int	main(void)
// {
// 	char	sys_func[10];
// 	char	my_func[10];
// 	int	i;
// 	i = 0;
// 	while (i < 9)
// 	{
// 		sys_func[i] = 'a';
// 		my_func[i] = 'b';
// 		i++;
// 	}
// 	sys_func[i] = '\0';
// 	my_func[i] = '\0';
// 	printf("Before sys func run: %s\n", sys_func);
// 	printf("Before my func runs: %s\n", my_func);
// 	bzero(sys_func, 10);
// 	ft_bzero(my_func, 10);

// 	printf("After sys func runs: %s\n", sys_func);
// 	printf("After my func runs: %s\n", my_func);
// }