/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:19:45 by pchowdry          #+#    #+#             */
/*   Updated: 2025/06/14 18:10:57 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;

	if (lst == NULL || *lst == NULL || del == NULL)
		return ;
	while (*lst)
	{
		temp = *lst;
		*lst = (*lst)->next;
		(*del)(temp->content);
		free(temp);
	}
	*lst = NULL;
}
