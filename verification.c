/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verification.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:39:31 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/29 23:18:37 by pchowdry         ###   ########.fr       */
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

char	is_wildcard_present(char *string)
{
	while (*string)
	{
		if (*string == '*')
			return (1);
		string++;
	}
	return (0);
}

t_list	*find_match_string(char *string_with_wildcard)
{
	char	**list_of_strings;
	t_list	*list_of_matches;

	if (string_with_wildcard == 0)
		return (0);
	list_of_strings = ft_split(string_with_wildcard, '*');
	if (list_of_strings == 0)
		return (0);
	list_of_matches = 0;
	if (ft_size(list_of_strings) != 0)
		list_of_matches = find_matches(ft_strncmp(string_with_wildcard,
					list_of_strings[0],
					ft_strlen(list_of_strings[0])) == 0,
				ft_strncmp(string_with_wildcard
					+ ft_strlen(string_with_wildcard)
					- ft_strlen(list_of_strings[ft_size(list_of_strings)
						- 1]), list_of_strings[ft_size(list_of_strings) - 1],
					ft_strlen(list_of_strings[ft_size(list_of_strings)
						- 1])) == 0, list_of_strings);
	else if (ft_strncmp("*", string_with_wildcard, 1) == 0)
		list_of_matches = find_matches(0, 0, list_of_strings);
	free_string_array(list_of_strings);
	return (list_of_matches);
}
