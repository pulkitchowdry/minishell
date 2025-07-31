/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:46:49 by pchowdry          #+#    #+#             */
/*   Updated: 2025/06/15 15:46:57 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_set(char c, char const *set)
{
	size_t	set_len;
	size_t	i;

	i = 0;
	set_len = ft_strlen(set);
	while (i < set_len)
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

size_t	ft_end(char const *s1, char const *set, size_t s1_len)
{
	size_t	i;

	i = s1_len - 1;
	while (i > 0)
	{
		if (is_set(s1[i], set))
			i--;
		else
			break ;
	}
	return (i);
}

size_t	ft_start(char const *s1, char const *set, size_t s1_len)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0' && i < s1_len)
	{
		if (is_set(s1[i], set))
			i++;
		else
			break ;
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trim;
	size_t	start;
	size_t	end;
	size_t	s1_len;
	size_t	i;

	i = 0;
	s1_len = ft_strlen(s1);
	if (s1_len == 0)
		return (ft_strdup(""));
	start = ft_start(s1, set, s1_len);
	end = ft_end(s1, set, s1_len);
	if (start > end)
		return (ft_strdup(""));
	trim = malloc((end - start + 2) * sizeof(char));
	if (!trim)
		return (NULL);
	while (start <= end)
	{
		trim[i] = s1[start];
		start++;
		i++;
	}
	trim[i] = '\0';
	return (trim);
}

// #include <stdio.h>
// int	main(void)
// {
// 	char	*res = ft_strtrim("abcdba","acb" );
// 	printf("My: %s\n", res);
// 	free(res);
// 	return (0);
// }