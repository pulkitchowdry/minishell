/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_initial_string.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:09:37 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/26 18:29:55 by chikoh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

int	process_initial_quotes_or_variable(t_token *cur_tok,
		t_ast_node **current, t_variable_context *context)
{
	if (cur_tok->type == DOUBLE_QUOTE_STRING)
		cur_tok->string = process_double_quote(cur_tok->string, context);
	else if (cur_tok->type == SINGLE_QUOTE_STRING)
		cur_tok->string = process_single_quote(cur_tok->string);
	else if (cur_tok->type == VARIABLE)
		cur_tok->string = process_variable(cur_tok->string, context);
	ft_lstadd_back(&((*current)->command),
		ft_lstnew(ft_strdup(cur_tok->string)));
	return (COMMAND_STRING);
}

int	process_initial_redirection(t_token *cur_tok, t_ast_node **current)
{
	ft_lstadd_back(&((*current)->command),
		ft_lstnew(ft_strdup(cur_tok->string)));
	return (COMMAND_REDIRECT);
}

int	process_initial_spaces(t_token *cur_tok, t_ast_node **current)
{
	ft_lstadd_back(&((*current)->command),
		ft_lstnew(ft_strdup(cur_tok->string)));
	return (COMMAND_SPACE);
}

int	process_initial_else(t_token *cur_tok, t_ast_node **current)
{
	ft_lstadd_back(&((*current)->command),
		ft_lstnew(ft_strdup(cur_tok->string)));
	return (EXIT);
}

int	process_initial_assignment(t_token *cur_tok, t_ast_node **current)
{
	ft_lstadd_back(&((*current)->assignment),
		ft_lstnew(ft_strdup(cur_tok->string)));
	return (ASSIGN_OP);
}
