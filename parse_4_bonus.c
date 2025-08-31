/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_4_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:12:22 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/31 15:22:04 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell_bonus.h"

#include <stdlib.h>

extern int	g_ret_code;

int	process_assign_string_val(t_list **list, t_token *cur_tok,
	t_ast_node **current, t_variable_context *context)
{
	t_token	*next_tok;

	if (is_command_string(cur_tok->type))
		append_string_to_current_node(cur_tok,
			(*current)->assignment, context);
	if (*list == 0)
		return (EXIT);
	next_tok = (t_token *)(*list)->content;
	if (is_command_string(next_tok->type))
		return (ASSIGN_STRING_VAL);
	else if (next_tok->type == HERE_DOC || next_tok->type == REDIRECT_APPEND
		|| next_tok->type == REDIRECT_OUTPUT
		|| next_tok->type == REDIRECT_INPUT)
		return (ASSIGN_REDIRECT);
	else if (next_tok->type == SPACES)
		return (ASSIGN_SPACE);
	else if (next_tok->type == PIPE || next_tok->type == LOGICAL_AND
		|| next_tok->type == LOGICAL_OR || next_tok->type == CLOSE_BRACKET)
		return (EXIT);
	else
	{
		free_command(current);
		return (EXIT);
	}
}

int	process_assign_space(t_list **list, t_ast_node **current)
{
	t_token	*next_tok;

	if (*list == 0)
		return (EXIT);
	next_tok = (t_token *)(*list)->content;
	if (next_tok->type == DOUBLE_QUOTE_STRING
		|| next_tok->type == SINGLE_QUOTE_STRING || next_tok->type == STRING)
		return (INITIAL_COMMAND_STRING);
	else if (is_redirect(next_tok->type))
		return (ASSIGN_REDIRECT);
	else if (next_tok->type == VARIABLE)
	{
		ft_lstadd_back(&((*current)->command), ft_lstnew(ft_strdup("")));
		return (COMMAND_STRING);
	}
	else if (next_tok->type == SPACES)
		return (ASSIGN_SPACE);
	else if (is_pipe_or_logical(next_tok->type))
		return (EXIT);
	else
	{
		free_command(current);
		return (EXIT);
	}
}

int	process_assign_op(t_list **list, t_ast_node **current)
{
	t_token	*next_tok;

	ft_lstadd_back(&(*current)->assignment, ft_lstnew(ft_strdup("")));
	if (*list == 0)
		return (EXIT);
	next_tok = (t_token *)(*list)->content;
	if (next_tok->type == DOUBLE_QUOTE_STRING
		|| next_tok->type == SINGLE_QUOTE_STRING
		|| next_tok->type == STRING || next_tok->type == VARIABLE)
		return (ASSIGN_STRING_VAL);
	else if (next_tok->type == SPACES)
		return (ASSIGN_SPACE);
	else if (is_redirect(next_tok->type))
		return (COMMAND_REDIRECT);
	else
	{
		free_command(current);
		return (EXIT);
	}
}

int	process_initial_command_string(t_list **list, t_token *cur_tok,
		t_ast_node **current, t_variable_context *context)
{
	t_token	*next_tok;

	if (*list == 0)
	{
		ft_lstadd_back(&((*current)->command),
			ft_lstnew(ft_strdup(cur_tok->string)));
		return (EXIT);
	}
	next_tok = (t_token *)(*list)->content;
	if (next_tok->type == ASSIGNMENT)
		return (process_initial_assignment(cur_tok, current));
	else if (next_tok->type == DOUBLE_QUOTE_STRING
		|| next_tok->type == SINGLE_QUOTE_STRING
		|| next_tok->type == VARIABLE)
		return (process_initial_quotes_or_variable(cur_tok, current, context));
	else if (is_redirect(next_tok->type))
		return (process_initial_redirection(cur_tok, current));
	else if (next_tok->type == SPACES)
		return (process_initial_spaces(cur_tok, current));
	else
		return (process_initial_else(cur_tok, current));
}

int	process_command_space(t_list **list, t_ast_node **current)
{
	t_token	*next_tok;

	if (*list == 0)
		return (EXIT);
	next_tok = (t_token *)(*list)->content;
	if (is_redirect(next_tok->type))
		return (COMMAND_REDIRECT);
	else if (next_tok->type == SPACES)
		return (COMMAND_SPACE);
	else if (next_tok->type == DOUBLE_QUOTE_STRING
		|| next_tok->type == SINGLE_QUOTE_STRING)
	{
		ft_lstadd_back(&((*current)->command), ft_lstnew(ft_strdup("")));
		return (COMMAND_QUOTES);
	}
	else if (next_tok->type == STRING || next_tok->type == ASSIGNMENT
		|| next_tok->type == VARIABLE)
	{
		ft_lstadd_back(&((*current)->command), ft_lstnew(ft_strdup("")));
		return (COMMAND_STRING);
	}
	else if (is_pipe_or_logical(next_tok->type))
		return (EXIT);
	else
		return (EXIT);
}
