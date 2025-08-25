/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_new_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chikoh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:56:58 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/25 19:55:24 by chikoh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

t_ast_node	*parse_new_node(t_state_context *state_context,
		t_token *cur_tok, t_list **list,
		t_ast_node *(*func)(t_list **, t_state_context *))
{
	t_ast_node	*current;

	current = (t_ast_node *)ft_calloc(sizeof(t_ast_node), 1);
	current->node = cur_tok;
	current->left = state_context->current;
	*list = (*list)->next;
	current->right = func(list, state_context);
	return (current);
}

char	is_extractable(int type)
{
	return (type == STRING || type == DOUBLE_QUOTE_STRING
		|| type == SINGLE_QUOTE_STRING || type == HERE_DOC
		|| type == REDIRECT_APPEND || type == REDIRECT_OUTPUT
		|| type == REDIRECT_INPUT || type == SPACES
		|| type == ASSIGNMENT || type == VARIABLE
		|| type == WILDCARD || type == ARGUMENTS
		|| type == COMMAND);
}

t_token	*advance_new_node(t_list **list)
{
	*list = (*list)->next;
	if (*list != 0)
		return ((t_token *)(*list)->content);
	else
		return (0);
}

t_ast_node	*free_and_return_zero(t_ast_node *current)
{
	free_command(&current);
	return (0);
}

t_ast_node	*parse_bracket(t_list **list, t_state_context *context)
{
	t_ast_node	*current;
	t_token		*cur_tok;

	*list = (*list)->next;
	current = parse_list(list, context);
	if (*list == 0)
		return (free_and_return_zero(current));
	cur_tok = (t_token *)(*list)->content;
	while (cur_tok->type == SPACES)
		cur_tok = advance_new_node(list);
	if (cur_tok->type == CLOSE_BRACKET)
		*list = (*list)->next;
	else
		free_command(&current);
	return (current);
}

char	is_command_redirect_space(int current_state)
{
	return (current_state == COMMAND_REDIRECT_SPACE
		|| current_state == ASSIGN_REDIRECT_SPACE);
}

char	is_command_redirect_string(int current_state)
{
	return (current_state == COMMAND_REDIRECT_STRING
		|| current_state == ASSIGN_REDIRECT_STRING);
}

char	is_command_redirect(int current_state)
{
	return (current_state == COMMAND_REDIRECT
		|| current_state == ASSIGN_REDIRECT);
}

int	process_state(int current_state, t_list **list,
		t_token *cur_tok, t_state_context *state_context)
{
	if (current_state == ASSIGN_STRING_VAL)
		return (process_assign_string_val(list, cur_tok,
				&state_context->current, state_context->context));
	else if (current_state == ASSIGN_SPACE)
		return (process_assign_space(list, &state_context->current));
	else if (current_state == ASSIGN_OP)
		return (process_assign_op(list, &state_context->current));
	else if (current_state == INITIAL_COMMAND_STRING)
		return (process_initial_command_string(list, cur_tok,
				&state_context->current, state_context->context));
	else if (current_state == COMMAND_SPACE)
		return (process_command_space(list, &state_context->current));
	else if (is_command_redirect(current_state))
		return (process_redirect(list, cur_tok,
				&state_context->current, current_state));
	else if (is_command_redirect_space(current_state))
		return (process_redirect_space(list,
				&state_context->current, current_state));
	else if (is_command_redirect_string(current_state))
		return (process_redirect_string(list,
				cur_tok, current_state, state_context));
	else if (current_state == COMMAND_QUOTES || current_state == COMMAND_STRING)
		return (process_command_string(list, cur_tok,
				&state_context->current, state_context->context));
	return (EXIT);
}

void	append_string_to_current_node(t_token *cur_tok,
		t_list *list, t_variable_context *context)
{
	char	*string;
	t_list	*last_node;

	if (cur_tok->type == DOUBLE_QUOTE_STRING)
		cur_tok->string = process_double_quote(cur_tok->string, context);
	else if (cur_tok->type == SINGLE_QUOTE_STRING)
		cur_tok->string = process_single_quote(cur_tok->string);
	else if (cur_tok->type == VARIABLE)
		cur_tok->string = process_variable(cur_tok->string, context);
	last_node = ft_lstlast(list);
	string = (char *)last_node->content;
	last_node->content = ft_strjoin(string, cur_tok->string);
	free(string);
}

char	is_command_string(int type)
{
	return (type == DOUBLE_QUOTE_STRING
		|| type == SINGLE_QUOTE_STRING
		|| type == STRING || type == ASSIGNMENT
		|| type == VARIABLE);
}

char	is_pipe_or_logical(int type)
{
	return (type == PIPE
		|| type == LOGICAL_AND
		|| type == LOGICAL_OR
		|| type == CLOSE_BRACKET);
}

char	is_redirect(int type)
{
	return (type == REDIRECT_INPUT || type == REDIRECT_OUTPUT
		|| type == REDIRECT_APPEND || type == HERE_DOC);
}
