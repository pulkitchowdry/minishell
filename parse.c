/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chikoh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 20:17:48 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/11 16:51:44 by chikoh           ###   ########.fr       */
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
	if (cur_tok->type == DOUBLE_QUOTE_STRING || cur_tok->type == SINGLE_QUOTE_STRING)
	{
		if (cur_tok->type == DOUBLE_QUOTE_STRING)
			process_double_quote(cur_tok->string);
		else if (cur_tok->type == SINGLE_QUOTE_STRING)
			process_single_quote(cur_tok->string);
		ft_lstadd_back(&((*current)->command), ft_lstnew(ft_strdup("")));
		return (COMMAND_QUOTES);
	}
	else if (cur_tok->type == HERE_DOC || cur_tok->type == REDIRECT_INPUT
			|| cur_tok->type == REDIRECT_OUTPUT || cur_tok->type == REDIRECT_APPEND)
		return (COMMAND_REDIRECT);
	else if (cur_tok->type == VARIABLE)
	{
		ft_lstadd_back(&((*current)->command), ft_lstnew(process_variable(cur_tok->string)));
		return (COMMAND_VARIABLE);
	}
	else if (cur_tok->type == SPACES)
		return (ASSIGN_SPACE);
	else if (cur_tok->type == STRING)
		return (INITIAL_COMMAND_STRING);
	else
	{
		free_command(current);
		return (EXIT);
	}	
}

int	process_assign_string_val(t_list **list, t_token *cur_tok, t_ast_node **current)
{
	t_token	*next_tok;
	t_list	*last_node;
	char	*string;

	if (cur_tok->type == DOUBLE_QUOTE_STRING || cur_tok->type == SINGLE_QUOTE_STRING
		|| cur_tok->type == STRING || cur_tok->type == ASSIGNMENT
		|| cur_tok->type == VARIABLE)
	{
		if (cur_tok->type == DOUBLE_QUOTE_STRING)
			process_double_quote(cur_tok->string);
		else if (cur_tok->type == SINGLE_QUOTE_STRING)
			process_single_quote(cur_tok->string);
		else if (cur_tok->type == VARIABLE)
			process_variable(cur_tok->string);
		last_node = ft_lstlast((*current)->assignment);
		string = (char *)last_node->content;
		last_node->content = ft_strjoin(string, cur_tok->string);
		free(string);
	}
	if (*list == 0)
		return (EXIT);
	next_tok = (t_token *)(*list)->content;
	if (next_tok->type == DOUBLE_QUOTE_STRING || next_tok->type == SINGLE_QUOTE_STRING
		|| next_tok->type == STRING || next_tok->type == ASSIGNMENT
		|| next_tok->type == VARIABLE)
		return (ASSIGN_STRING_VAL);
	else if (next_tok->type == HERE_DOC || next_tok->type == REDIRECT_APPEND
		|| next_tok->type == REDIRECT_OUTPUT || next_tok->type == REDIRECT_INPUT)
		return (COMMAND_REDIRECT);
	else if (next_tok->type == SPACES)
		return (ASSIGN_SPACE);
	else if (next_tok->type == PIPE || next_tok->type == LOGICAL_AND || next_tok->type == LOGICAL_OR || next_tok->type == CLOSE_BRACKET)
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
	if (next_tok->type == DOUBLE_QUOTE_STRING || next_tok->type == SINGLE_QUOTE_STRING
		|| next_tok->type == STRING)
		return (INITIAL_COMMAND_STRING);
	else if (next_tok->type == REDIRECT_INPUT || next_tok->type == REDIRECT_OUTPUT
		|| next_tok->type == REDIRECT_APPEND || next_tok->type == HERE_DOC)
		return (COMMAND_REDIRECT);
	else if (next_tok->type == VARIABLE)
		return (COMMAND_STRING);
	else if (next_tok->type == SPACES)
		return (ASSIGN_SPACE);
	else if (next_tok->type == PIPE || next_tok->type == LOGICAL_AND || next_tok->type == LOGICAL_OR || next_tok->type == CLOSE_BRACKET)
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
	if (next_tok->type == DOUBLE_QUOTE_STRING || next_tok->type == SINGLE_QUOTE_STRING
		|| next_tok->type == STRING || next_tok->type == VARIABLE)
		return (ASSIGN_STRING_VAL);
	else if (next_tok->type == SPACES)
		return (ASSIGN_SPACE);
	else if (next_tok->type == REDIRECT_INPUT || next_tok->type == REDIRECT_OUTPUT
		|| next_tok->type == REDIRECT_APPEND || next_tok->type == HERE_DOC)
		return (COMMAND_REDIRECT);
	else
	{
		free_command(current);
		return (EXIT);
	}	
}

int	process_initial_command_string(t_list **list, t_token *cur_tok, t_ast_node **current)
{
	t_token	*next_tok;

	if (*list == 0)
		return (EXIT);
	next_tok = (t_token *)(*list)->content;
	if (next_tok->type == ASSIGNMENT)
	{
		ft_lstadd_back(&((*current)->assignment), ft_lstnew(cur_tok->string));
		return (ASSIGN_OP);
	}
	else if (next_tok->type == DOUBLE_QUOTE_STRING || next_tok->type == SINGLE_QUOTE_STRING)
		return (COMMAND_STRING);
	else if (next_tok->type == REDIRECT_INPUT || next_tok->type == REDIRECT_OUTPUT
		|| next_tok->type == REDIRECT_APPEND || next_tok->type == HERE_DOC)
	{
		ft_lstadd_back(&((*current)->redirection), ft_lstnew(cur_tok->string));
		return (COMMAND_REDIRECT);
	}
	else if (next_tok->type == SPACES)
	{
		ft_lstadd_back(&((*current)->command), ft_lstnew(cur_tok->string));
		return (COMMAND_SPACE);
	}
	else if (next_tok->type == PIPE || next_tok->type == LOGICAL_AND || next_tok->type == LOGICAL_OR || next_tok->type == CLOSE_BRACKET)
		return (EXIT);
	else
	{
		free_command(current);
		return (EXIT);
	}	
}

int	process_command_space(t_list **list, t_ast_node **current)
{
	t_token	*next_tok;

	if (*list == 0)
		return (EXIT);
	next_tok = (t_token *)(*list)->content;
	if (next_tok->type == REDIRECT_INPUT || next_tok->type == REDIRECT_OUTPUT
		|| next_tok->type == REDIRECT_APPEND || next_tok->type == HERE_DOC)
		return (COMMAND_REDIRECT);
	else if (next_tok->type == SPACES)
		return (COMMAND_SPACE);
	else if (next_tok->type == DOUBLE_QUOTE_STRING || next_tok->type == SINGLE_QUOTE_STRING)
		return (COMMAND_QUOTES);
	else if (next_tok->type == STRING || next_tok->type == ASSIGNMENT || next_tok->type == VARIABLE)
	{
		ft_lstadd_back(&((*current)->command), ft_lstnew(ft_strdup("")));
		return (COMMAND_STRING);
	}
	else if (next_tok->type == PIPE || next_tok->type == LOGICAL_AND || next_tok->type == LOGICAL_OR || next_tok->type == CLOSE_BRACKET)
		return (EXIT);
	else
	{
		free_command(current);
		return (EXIT);
	}	
}

int	process_command_redirect(t_list **list, t_token *cur_tok, t_ast_node **current)
{
	t_token	*next_tok;

	ft_lstadd_back(&((*current)->redirection), ft_lstnew(cur_tok->string));
	ft_lstadd_back(&((*current)->redirection), ft_lstnew(ft_strdup("")));
	if (*list == 0)
		return (EXIT);
	next_tok = (t_token *)(*list)->content;
	if (next_tok->type == SPACES)
		return (REDIRECT_SPACE);
	else if (next_tok->type == DOUBLE_QUOTE_STRING || next_tok->type == SINGLE_QUOTE_STRING
		|| next_tok->type == STRING || next_tok->type == ASSIGNMENT)
		return (REDIRECT_STRING);
	else
	{
		free_command(current);
		return (EXIT);
	}	
}

int	process_command_string(t_list **list, t_token *cur_tok, t_ast_node **current)
{
	t_token	*next_tok;
	char	*string;
	t_list	*last_node;

	if (cur_tok->type == DOUBLE_QUOTE_STRING || cur_tok->type == SINGLE_QUOTE_STRING
		|| cur_tok->type == STRING || cur_tok->type == ASSIGNMENT
		|| cur_tok->type == VARIABLE)
	{
		if (cur_tok->type == DOUBLE_QUOTE_STRING)
			process_double_quote(cur_tok->string);
		else if (cur_tok->type == SINGLE_QUOTE_STRING)
			process_single_quote(cur_tok->string);
		else if (cur_tok->type == VARIABLE)
			process_variable(cur_tok->string);
		last_node = ft_lstlast((*current)->command);
		string = (char *)last_node->content;
		string = ft_strjoin(string, cur_tok->string);
		free(last_node->content);
		last_node->content = string;
	}
	if (*list == 0)
		return (EXIT);
	next_tok = (t_token *)(*list)->content;
	if (next_tok->type == ASSIGNMENT || next_tok->type == DOUBLE_QUOTE_STRING
			|| next_tok->type == SINGLE_QUOTE_STRING || next_tok->type == VARIABLE
			|| next_tok->type == STRING)
		return (COMMAND_STRING);
	else if (next_tok->type == REDIRECT_INPUT || next_tok->type == REDIRECT_OUTPUT
		|| next_tok->type == REDIRECT_APPEND || next_tok->type == HERE_DOC)
		return (COMMAND_REDIRECT);
	else if (next_tok->type == SPACES)
		return (COMMAND_SPACE);
	else if (next_tok->type == PIPE || next_tok->type == LOGICAL_AND || next_tok->type == LOGICAL_OR || next_tok->type == CLOSE_BRACKET)
		return (EXIT);
	else
	{
		free_command(current);
		return (EXIT);
	}
}

int	process_redirect_string(t_list **list, t_token *cur_tok, t_ast_node **current)
{
	t_token	*next_tok;
	char	*string;
	t_list	*last_node;

	if (cur_tok->type == DOUBLE_QUOTE_STRING || cur_tok->type == SINGLE_QUOTE_STRING
		|| cur_tok->type == STRING || cur_tok->type == ASSIGNMENT
		|| cur_tok->type == VARIABLE)
	{
		if (cur_tok->type == DOUBLE_QUOTE_STRING)
			process_double_quote(cur_tok->string);
		else if (cur_tok->type == SINGLE_QUOTE_STRING)
			process_single_quote(cur_tok->string);
		else if (cur_tok->type == VARIABLE)
			process_variable(cur_tok->string);
		last_node = ft_lstlast((*current)->redirection);
		string = (char *)last_node->content;
		string = ft_strjoin(string, cur_tok->string);
		free(last_node->content);
		last_node->content = string;
	}
	if (*list == 0)
		return (EXIT);
	next_tok = (t_token *)(*list)->content;
	if (next_tok->type == SPACES)
		return (COMMAND_SPACE);
	else if (next_tok->type == DOUBLE_QUOTE_STRING || next_tok->type == SINGLE_QUOTE_STRING
		|| next_tok->type == ASSIGNMENT)
		return (REDIRECT_STRING);
	else
	{
		free_command(current);
		return (EXIT);
	}	
}

int	process_redirect_space(t_list **list, t_ast_node **current)
{
	t_token	*next_tok;

	if (*list == 0)
		return (EXIT);
	next_tok = (t_token *)(*list)->content;
	if (next_tok->type == SPACES)
		return (REDIRECT_SPACE);
	else if (next_tok->type == DOUBLE_QUOTE_STRING || next_tok->type == SINGLE_QUOTE_STRING
		|| next_tok->type == STRING || next_tok->type == ASSIGNMENT || next_tok->type == VARIABLE)
		return (REDIRECT_STRING);
	else
	{
		free_command(current);
		return (EXIT);
	}	
}

t_ast_node	*extract_command(t_list **list)
{
	t_ast_node	*current;
	t_token	*cur_tok;
	int	current_state;

	cur_tok = (t_token *)(*list)->content;
	current = (t_ast_node *)ft_calloc(sizeof(t_ast_node), 1);
	current_state = initialize_command_state(cur_tok, &current);
	while (current_state != EXIT)
	{
		cur_tok = (t_token *)(*list)->content;
		*list = (*list)->next;
		if (current_state == ASSIGN_STRING_VAL)
			current_state = process_assign_string_val(list, cur_tok, &current);
		else if (current_state == ASSIGN_SPACE)
			current_state = process_assign_space(list, &current);
		else if (current_state == ASSIGN_OP)
			current_state = process_assign_op(list, &current);
		else if (current_state == INITIAL_COMMAND_STRING)
			current_state = process_initial_command_string(list, cur_tok, &current);
		else if (current_state == COMMAND_SPACE)
			current_state = process_command_space(list, &current);
		else if (current_state == COMMAND_REDIRECT)
			current_state = process_command_redirect(list, cur_tok, &current);
		else if (current_state == REDIRECT_SPACE)
			current_state = process_redirect_space(list, &current);
		else if (current_state == REDIRECT_STRING)
			current_state = process_redirect_string(list, cur_tok, &current);
		else if (current_state == COMMAND_QUOTES || current_state == COMMAND_STRING)
			current_state = process_command_string(list, cur_tok, &current);
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
	while (cur_tok->type == SPACES)
	{
		*list = (*list)->next;
		cur_tok = (t_token *)(*list)->content;
	}
	if (cur_tok->type == OPEN_BRACKET)
	{
		*list = (*list)->next;
		current = parse_list(list);
		cur_tok = (t_token *)(*list)->content;
		while (cur_tok->type == SPACES)
		{
			*list = (*list)->next;
			cur_tok = (t_token *)(*list)->content;
		}
		if (cur_tok->type == CLOSE_BRACKET)
			*list = (*list)->next;
		else
			free_command(&current);
		return (current);
	}
	else if (cur_tok->type == STRING || 
			cur_tok->type == DOUBLE_QUOTE_STRING || 
			cur_tok->type == SINGLE_QUOTE_STRING || 
			cur_tok->type == HERE_DOC || 
			cur_tok->type == REDIRECT_APPEND || 
			cur_tok->type == REDIRECT_OUTPUT || 
			cur_tok->type == REDIRECT_INPUT || 
			cur_tok->type == SPACES || 
			cur_tok->type == ASSIGNMENT || 
			cur_tok->type == VARIABLE || 
			cur_tok->type == WILDCARD || 
			cur_tok->type == ARGUMENTS || 
			cur_tok->type == COMMAND)
		current = extract_command(list);
	return (current);
}

t_ast_node	*parse_pipeline(t_list **list)
{
	t_ast_node	*left_node;
	t_ast_node	*current;
	t_token	*cur_tok;

	left_node = parse_command(list);
	if (left_node == 0)
		return (0);
	if (*list == 0)
		return (left_node);
	current = 0;
	cur_tok = (t_token *)(*list)->content;
	while (cur_tok->type == SPACES || cur_tok->type == PIPE)
	{
		while (cur_tok->type == SPACES)
		{
			*list = (*list)->next;
			cur_tok = (t_token *)(*list)->content;
		}
		if (cur_tok->type == PIPE)
		{
			current = (t_ast_node *)ft_calloc(sizeof(t_ast_node), 1);
			current->node = cur_tok;
			current->left = left_node;
			*list = (*list)->next;
			current->right = parse_command(list);
			left_node = current;
		}
		if (*list == 0)
			break ;
		cur_tok = (t_token *)(*list)->content;
	}
	return (left_node);
}

t_ast_node	*parse_list(t_list **list)
{
	t_ast_node	*left_node;
	t_ast_node	*current;
	t_token	*cur_tok;

	left_node = parse_pipeline(list);
	if (left_node == 0)
		return (0);
	if (*list == 0)
		return (left_node);
	current = 0;
	cur_tok = (t_token *)(*list)->content;
	while (cur_tok->type == SPACES || cur_tok->type == LOGICAL_OR || cur_tok->type == LOGICAL_AND)
	{
		while (cur_tok->type == SPACES)
		{
			*list = (*list)->next;
			cur_tok = (t_token *)(*list)->content;
		}
		if (cur_tok->type == LOGICAL_AND || cur_tok->type == LOGICAL_OR)
		{
			current = (t_ast_node *)ft_calloc(sizeof(t_ast_node), 1);
			current->node = cur_tok;
			current->left = left_node;
			*list = (*list)->next;
			current->right = parse_pipeline(list);
			left_node = current;
		}
		if (*list == 0)
			break ;
		cur_tok = (t_token *)(*list)->content;
	}
	return (left_node);
}
