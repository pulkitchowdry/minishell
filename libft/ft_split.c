/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 09:35:43 by pchowdry          #+#    #+#             */
/*   Updated: 2025/06/15 15:26:27 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// int	is_sep(char s, char c)
// {
// 	if (s == c)
// 		return (1);
// 	return (0);
// }

int	ft_find_word(char const *s, char c, int i)
{
	while (s[i] != '\0' && (s[i] == c))
		i++;
	return (i);
}

size_t	ft_strlen_2(char const *s, char c)
{
	size_t	i;
	size_t	len;
	size_t	count_word;

	i = 0;
	len = 0;
	count_word = 1;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			count_word = 1;
		if ((s[i] != c) && count_word == 1)
		{
			len++;
			count_word = 0;
		}
		i++;
	}
	return (len);
}

void	ft_split_free(char **str, size_t j)
{
	while (j > 0)
	{
		j--;
		free(str[j]);
	}
	free(str);
}

char	*ft_strndup(char const *start, size_t len)
{
	char	*sub;
	size_t	i;

	i = 0;
	sub = malloc((len + 1) * sizeof(char));
	if (!sub)
		return (NULL);
	while (i < len)
	{
		sub[i] = start[i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	size_t	i;
	size_t	j;
	size_t	start;

	i = 0;
	j = 0;
	str = malloc((ft_strlen_2(s, c) + 1) * sizeof(char *));
	if (!str)
		return (NULL);
	while (s[i] != '\0')
	{
		i = ft_find_word(s, c, i);
		start = i;
		while (s[i] != '\0' && (s[i] != c))
			i++;
		if (i > start)
		{
			str[j] = ft_strndup(&s[start], i - start);
			if (!str[j++])
				return (ft_split_free(str, j), NULL);
		}
	}
	str[j] = NULL;
	return (str);
}

// #include <stdio.h>
// int	main(void)
// {
// 	char	**res = ft_split("blah blah blah", ' ');
// 	int	i;

// 	i = 0;
// 	while (res[i])
// 	{
// 		printf("My: %s\n", res[i]);
// 		free(res[i]);
// 		i++;
// 	}
// 	free(res);
// 	return (0);
// }