/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:09:03 by pchowdry          #+#    #+#             */
/*   Updated: 2025/05/10 16:27:40 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_display_num(long num, int fd)
{
	if (num >= 10)
		ft_display_num(num / 10, fd);
	ft_putchar_fd((num % 10 + '0'), fd);
}

void	ft_putnbr_fd(int n, int fd)
{
	long	num;

	num = n;
	if (num < 0)
	{
		write(fd, "-", 1);
		num = -num;
	}
	ft_display_num(num, fd);
}

// int	main(void)
// {
// 	ft_putnbr_fd(2, 1);
// 	return (0);
// }