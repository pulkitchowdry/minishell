/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chikoh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 20:17:48 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/16 22:07:38 by chikoh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

#include <stdlib.h>

char	*process_double_quote(char *string)
{
	return (string);
}

char	*process_single_quote(char *string)
{
	return (string);
}

char	*process_variable(char *string)
{
	return (string);
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

int	process_assign_string_val(t_list **list, t_token *cur_tok,
	t_ast_node **current)
{
	t_token	*next_tok;

	if (is_command_string(cur_tok->type))
		append_string_to_current_node(*current,
			cur_tok, (*current)->assignment);
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

int	process_initial_command_string(t_list **list,
		t_token *cur_tok, t_ast_node **current)
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
		return (process_initial_quotes_or_variable(cur_tok, current));
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
		return (COMMAND_QUOTES);
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

int	process_redirect(t_list **list, t_token *cur_tok,
		t_ast_node **current, int current_state)
{
	t_token	*next_tok;

	ft_lstadd_back(&((*current)->redirection),
		ft_lstnew(ft_strdup(cur_tok->string)));
	ft_lstadd_back(&((*current)->redirection), ft_lstnew(ft_strdup("")));
	if (*list == 0)
		return (EXIT);
	next_tok = (t_token *)(*list)->content;
	if (next_tok->type == SPACES)
		return (determine_next_redirect_if_space(current_state));
	else if (is_command_string(next_tok->type))
		return (determine_next_redirect_if_command_string(current_state));
	else if (is_redirect(next_tok->type))
		return (determine_next_redirect_string_if_redirect(current_state));
	else
	{
		free_command(current);
		return (EXIT);
	}
}

int	process_command_string(t_list **list,
		t_token *cur_tok, t_ast_node **current)
{
	t_token	*next_tok;

	if (is_command_string(cur_tok->type))
		append_string_to_current_node(*current, cur_tok, (*current)->command);
	if (*list == 0)
		return (EXIT);
	next_tok = (t_token *)(*list)->content;
	if (is_command_string(next_tok->type))
		return (COMMAND_STRING);
	else if (is_redirect(next_tok->type))
		return (COMMAND_REDIRECT);
	else if (next_tok->type == SPACES)
		return (COMMAND_SPACE);
	else if (is_pipe_or_logical(next_tok->type))
		return (EXIT);
	else
	{
		free_command(current);
		return (EXIT);
	}
}

int	process_redirect_string(t_list **list, t_token *cur_tok,
		t_ast_node **current, int current_state)
{
	t_token	*next_tok;

	if (is_command_string(cur_tok->type))
		append_string_to_current_node(*current,
			cur_tok, (*current)->redirection);
	if (*list == 0)
		return (EXIT);
	next_tok = (t_token *)(*list)->content;
	if (next_tok->type == SPACES)
		return (determine_next_redirect_string_if_space(current_state));
	else if (is_command_string(next_tok->type))
		return (determine_next_redirect_string_if_command(current_state));
	else if (is_redirect(next_tok->type))
		return (determine_next_redirect_string_if_redirect(current_state));
	else if (is_pipe_or_logical(next_tok->type)
		|| next_tok->type == CLOSE_BRACKET)
		return (EXIT);
	else
		return (EXIT);
}

int	process_redirect_space(t_list **list,
		t_ast_node **current, int current_state)
{
	if (*list == 0)
		return (EXIT);
	if (((t_token *)(*list)->content)->type == SPACES)
	{
		if (current_state == ASSIGN_REDIRECT_SPACE)
			return (ASSIGN_REDIRECT_SPACE);
		else if (current_state == COMMAND_REDIRECT_SPACE)
			return (COMMAND_REDIRECT_SPACE);
		return (EXIT);
	}
	else if (is_command_string(((t_token *)(*list)->content)->type))
	{
		if (current_state == ASSIGN_REDIRECT_SPACE)
			return (ASSIGN_REDIRECT_STRING);
		else if (current_state == COMMAND_REDIRECT_SPACE)
			return (COMMAND_REDIRECT_STRING);
		return (EXIT);
	}
	else
	{
		free_command(current);
		return (EXIT);
	}
}

t_ast_node	*extract_command(t_list **list)
{
	t_ast_node	*current;
	t_token		*cur_tok;
	int			current_state;

	cur_tok = (t_token *)(*list)->content;
	current = (t_ast_node *)ft_calloc(sizeof(t_ast_node), 1);
	current_state = initialize_command_state(cur_tok, &current);
	while (current_state != EXIT)
	{
		cur_tok = (t_token *)(*list)->content;
		*list = (*list)->next;
		current_state = process_state(current_state, list,
				cur_tok, &current);
	}
	return (current);
}

t_ast_node	*parse_command(t_list **list)
{
	t_ast_node	*current;
	t_token		*cur_tok;

	if (*list == 0)
		return (0);
	current = 0;
	cur_tok = (t_token *)(*list)->content;
	while (cur_tok != 0 && cur_tok->type == SPACES)
		cur_tok = advance_new_node(list);
	if (cur_tok != 0 && cur_tok->type == OPEN_BRACKET)
		return (parse_bracket(list));
	else if (cur_tok != 0 && is_extractable(cur_tok->type))
		current = extract_command(list);
	return (current);
}

t_ast_node	*parse_pipeline(t_list **list)
{
	t_ast_node	*left_node;
	t_token		*cur_tok;

	left_node = parse_command(list);
	if (left_node == 0)
		return (0);
	if (*list == 0)
		return (left_node);
	cur_tok = (t_token *)(*list)->content;
	while (cur_tok != 0 && (cur_tok->type == SPACES || cur_tok->type == PIPE))
	{
		while (cur_tok != 0 && cur_tok->type == SPACES)
			cur_tok = advance_new_node(list);
		if (cur_tok == 0)
			return (0);
		if (cur_tok->type == PIPE)
			left_node = parse_new_node(left_node, cur_tok, list, parse_command);
		if (*list == 0)
			break ;
		cur_tok = (t_token *)(*list)->content;
	}
	return (left_node);
}

t_ast_node	*parse_list(t_list **list)
{
	t_ast_node	*left_node;
	t_token		*cur_tok;

	left_node = parse_pipeline(list);
	if (left_node == 0)
		return (0);
	if (*list == 0)
		return (left_node);
	cur_tok = (t_token *)(*list)->content;
	while (cur_tok->type == SPACES || cur_tok->type == LOGICAL_OR
		|| cur_tok->type == LOGICAL_AND)
	{
		while (cur_tok != 0 && cur_tok->type == SPACES)
			cur_tok = advance_new_node(list);
		if (cur_tok == 0)
			return (0);
		if (cur_tok->type == LOGICAL_AND || cur_tok->type == LOGICAL_OR)
			left_node = parse_new_node(left_node, cur_tok,
					list, parse_pipeline);
		if (*list == 0)
			break ;
		cur_tok = (t_token *)(*list)->content;
	}
	return (left_node);
}
