/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_3_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:11:50 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/31 15:22:01 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell_bonus.h"

#include <stdlib.h>

extern int	g_ret_code;

char	*process_single_quote(char *string)
{
	return (ft_strtrim(string, "'"));
}

char	*process_variable(char *string, t_variable_context *context)
{
	char	*end;
	char	*start;
	char	curly_brackets;

	start = ft_strchr(string, '$');
	end = start + 1;
	curly_brackets = *end == '{';
	end += curly_brackets;
	while ((ft_isalnum(*end) || *end == '_'
			|| (curly_brackets && *end == '}') || *end == '?') && *end)
	{
		end++;
		if (curly_brackets && *end == '}')
			break ;
	}
	return (substitute_variable(string, start + 1,
			end + (curly_brackets && *end == '}'), context));
}

char	*process_wildcard(t_ast_node *current_node, char *string, t_list *list)
{
	(void)current_node;
	(void)list;
	return (string);
}

void	free_command(t_ast_node **current)
{
	if (*current == 0)
		return ;
	(*current)->node = 0;
	ft_lstclear(&(*current)->command, free);
	ft_lstclear(&(*current)->redirection, free);
	ft_lstclear(&(*current)->assignment, free);
	free_command(&(*current)->left);
	free_command(&(*current)->right);
	free(*current);
	*current = 0;
}

int	initialize_command_state(t_token *cur_tok, t_ast_node **current)
{
	if (cur_tok->type == DOUBLE_QUOTE_STRING
		|| cur_tok->type == SINGLE_QUOTE_STRING)
	{
		ft_lstadd_back(&((*current)->command), ft_lstnew(ft_strdup("")));
		return (COMMAND_QUOTES);
	}
	else if (is_redirect(cur_tok->type))
		return (ASSIGN_REDIRECT);
	else if ((cur_tok->type == STRING && is_wildcard_present(cur_tok->string))
		|| (cur_tok->type == VARIABLE))
	{
		ft_lstadd_back(&((*current)->command), ft_lstnew(ft_strdup("")));
		return (COMMAND_STRING);
	}
	else if (cur_tok->type == SPACES)
		return (ASSIGN_SPACE);
	else if (cur_tok->type == STRING && !is_wildcard_present(cur_tok->string))
		return (INITIAL_COMMAND_STRING);
	else
	{
		free_command(current);
		return (EXIT);
	}
}
