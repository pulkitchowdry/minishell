/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 09:55:05 by pchowdry          #+#    #+#             */
/*   Updated: 2025/06/14 18:16:30 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new_node;

	new_node = malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}

// #include <stdio.h>
// int main(void)
// {
//     int    *content = malloc(sizeof(int));
//     if (!content)
//         return (0);
//     *content = 42;
//     t_list *res = ft_lstnew(content);
//     printf("Content: %d", *(int *)res->content);
//     free(content);
//     free(res);
//     return (0);
// }