/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 20:17:48 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/26 18:29:25 by chikoh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

#include <stdlib.h>

char	*extract_variable_name(char *string, char *start, char *end)
{
	if (*start == '{')
	{
		start++;
		end--;
		end += *end != '}';
	}
	return (ft_substr(string, start - string, end - start));
}

char	*get_variable_key(char *string)
{
	char	*equal_sign;

	equal_sign = ft_strchr(string, '=');
	if (equal_sign == 0)
		return (ft_strdup(""));
	return (ft_substr(string, 0, equal_sign - string));
}

char	**set_variable_string(char **variable_list, char *string)
{
	char	*key;
	char	*new_key;
	int		seek;
	char	match;

	seek = 0;
	while (variable_list[seek])
	{
		key = get_variable_key(variable_list[seek]);
		new_key = get_variable_key(string);
		match = ft_strncmp(key, new_key, ft_strlen(new_key) + 1) == 0;
		free(key);
		free(new_key);
		if (match)
		{
			free(variable_list[seek]);
			variable_list[seek] = string;
			return (variable_list);
		}
	}
	return (append_string_array(variable_list, string));
}

char	*get_variable_value(char *string)
{
	char	*equal_sign;

	equal_sign = ft_strchr(string, '=');
	if (equal_sign == 0)
		return (ft_strdup(""));
	return (ft_substr(string, equal_sign - string + 1,
			ft_strlen(equal_sign + 1)));
}

char	*get_special_variable(char *variable)
{
	if (ft_strncmp(variable, "?", 2) == 0)
		return (ft_itoa(0));
	return (ft_strdup(""));
}

char	is_variable_match(char *string, char *variable)
{
	char	*variable_key;
	char	match;

	variable_key = get_variable_key(string);
	match = ft_strncmp(variable_key, variable,
			ft_strlen(variable_key) + 1) == 0;
	free(variable_key);
	return (match);
}

char	*get_value(t_variable_context *context, char *variable)
{
	int		seek;

	seek = 0;
	while (context->environment_variables[seek])
	{
		if (is_variable_match(context->environment_variables[seek], variable))
			return (get_variable_value(context->environment_variables[seek]));
		seek++;
	}
	seek = 0;
	while (context->local_variables && context->local_variables[seek])
	{
		if (is_variable_match(context->local_variables[seek], variable))
			return (get_variable_value(context->local_variables[seek]));
		seek++;
	}
	return (get_special_variable(variable));
}

char	*free_and_return_original_string(char *string, char *head, char *tail)
{
	free(head);
	free(tail);
	return (string);
}

char	*substitute_variable(char *string, char *start,
	char *end, t_variable_context *context)
{
	char	*result;
	char	*head;
	char	*variable;
	char	*value;
	char	*tail;

	head = ft_substr(string, 0, start - string - 1);
	tail = ft_substr(string, end - string, ft_strlen(string) - (end - string));
	variable = extract_variable_name(string, start, end);
	if (variable == 0)
		return (free_and_return_original_string(string, head, tail));
	value = get_value(context, variable);
	free(variable);
	result = (char *)ft_calloc(sizeof(char), ft_strlen(head) + ft_strlen(value)
			+ ft_strlen(tail) + 1);
	ft_strlcat(result, head, ft_strlen(head) + 1);
	ft_strlcat(result, value, ft_strlen(head) + ft_strlen(value) + 1);
	ft_strlcat(result, tail, ft_strlen(head)
		+ ft_strlen(value) + ft_strlen(tail) + 1);
	free(head);
	free(value);
	free(tail);
	free(string);
	return (result);
}

char	*process_double_quote(char *string, t_variable_context *context)
{
	char	*end;
	char	*start;
	char	curly_brackets;
	char	*result;

	result = ft_strtrim(string, "\"");
	free(string);
	start = ft_strchr(result, '$');
	while (start)
	{
		end = start + 1;
		curly_brackets = *end == '{';
		end += curly_brackets;
		while ((ft_isalnum(*end) || *end == '_'
				|| (curly_brackets && *end == '}')) && *end)
		{
			end++;
			if (curly_brackets && *end == '}')
				break ;
		}
		result = substitute_variable(result, start + 1,
				end + (curly_brackets && *end == '}'), context);
		start = ft_strchr(end, '$');
	}
	return (result);
}

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
			|| (curly_brackets && *end == '}')) && *end)
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

int	process_redirect(t_list **list, t_token *cur_tok,
		t_ast_node **current, int current_state)
{
	t_token	*next_tok;

	ft_lstadd_back(&((*current)->redirection),
		ft_lstnew(ft_strdup(cur_tok->string)));
	ft_lstadd_back(&((*current)->redirection), ft_lstnew(ft_strdup("")));
	if (*list == 0)
	{
		free_command(current);
		return (EXIT);
	}
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

int	process_command_string(t_list **list, t_token *cur_tok,
		t_ast_node **current, t_variable_context *context)
{
	t_token	*next_tok;

	if (is_command_string(cur_tok->type))
		append_string_to_current_node(cur_tok, (*current)->command,
			context);
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
		int current_state, t_state_context *state_context)
{
	t_token	*next_tok;

	if (is_command_string(cur_tok->type))
		append_string_to_current_node(cur_tok,
			(*state_context->current).redirection, state_context->context);
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
	{
		free_command(current);
		return (EXIT);
	}
	if (((t_token *)(*list)->content)->type == SPACES)
		return (determine_next_redirect_space_if_space(current_state));
	else if (is_command_string(((t_token *)(*list)->content)->type))
		return (determine_next_redirect_space_if_string(current_state));
	else
	{
		free_command(current);
		return (EXIT);
	}
}

t_ast_node	*extract_command(t_list **list, t_state_context *context)
{
	t_ast_node	*current;
	t_token		*cur_tok;
	int			current_state;

	cur_tok = (t_token *)(*list)->content;
	current = (t_ast_node *)ft_calloc(sizeof(t_ast_node), 1);
	context->current = current;
	current_state = initialize_command_state(cur_tok, &current);
	while (current_state != EXIT)
	{
		cur_tok = (t_token *)(*list)->content;
		*list = (*list)->next;
		current_state = process_state(current_state, list,
				cur_tok, context);
	}
	return (current);
}

t_ast_node	*parse_command(t_list **list, t_state_context *context)
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
		return (parse_bracket(list, context));
	else if (cur_tok != 0 && is_extractable(cur_tok->type))
		current = extract_command(list, context);
	return (current);
}

t_ast_node	*parse_pipeline(t_list **list, t_state_context *context)
{
	t_token		*cur_tok;

	context->current = parse_command(list, context);
	if (context->current == 0)
		return (0);
	if (*list == 0)
		return (context->current);
	cur_tok = (t_token *)(*list)->content;
	while (cur_tok != 0 && (cur_tok->type == SPACES || cur_tok->type == PIPE))
	{
		while (cur_tok != 0 && cur_tok->type == SPACES)
			cur_tok = advance_new_node(list);
		if (cur_tok == 0)
			return (0);
		if (cur_tok->type == PIPE)
			context->current = parse_new_node(context,
					cur_tok, list, parse_command);
		if (*list == 0)
			break ;
		cur_tok = (t_token *)(*list)->content;
	}
	return (context->current);
}

char	is_space_or_logical(t_token *cur_tok)
{
	return (cur_tok->type == SPACES || cur_tok->type == LOGICAL_OR
		|| cur_tok->type == LOGICAL_AND);
}

t_ast_node	*parse_list(t_list **list, t_state_context *context)
{
	t_token		*cur_tok;

	context->current = parse_pipeline(list, context);
	if (context->current == 0)
		return (0);
	if (*list == 0)
		return (context->current);
	cur_tok = (t_token *)(*list)->content;
	while (cur_tok->type == SPACES || cur_tok->type == LOGICAL_OR
		|| cur_tok->type == LOGICAL_AND)
	{
		while (cur_tok != 0 && cur_tok->type == SPACES)
			cur_tok = advance_new_node(list);
		if (cur_tok == 0)
			return (0);
		if (cur_tok->type == LOGICAL_AND || cur_tok->type == LOGICAL_OR)
			context->current = parse_new_node(context, cur_tok,
					list, parse_pipeline);
		if (*list == 0)
			break ;
		cur_tok = (t_token *)(*list)->content;
	}
	return (context->current);
}
