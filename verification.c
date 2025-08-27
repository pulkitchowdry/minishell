/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verification.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:39:31 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/27 22:56:41 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

extern int	g_ret_code;

char	is_safe_to_execute(t_ast_node *node)
{
	if (node == 0)
		return (1);
	if (node->node != 0 && (node->left == 0 || node->right == 0))
		return (0);
	return (is_safe_to_execute(node->left) && is_safe_to_execute(node->right));
}
