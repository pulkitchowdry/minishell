/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 15:44:46 by pchowdry          #+#    #+#             */
/*   Updated: 2025/06/14 18:19:32 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int c, size_t n)
{
	unsigned char	*ptr;
	size_t			i;

	ptr = (unsigned char *) str;
	i = 0;
	while (i < n)
	{
		ptr[i] = (unsigned char) c;
		i++;
	}
	return (str);
}

// #include <string.h>
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
// 	memset(sys_func, 67, 1);
// 	ft_memset(my_func, 68, 1);

// 	printf("After sys func runs: %s\n", sys_func);
// 	printf("After my func runs: %s\n", my_func);
// }