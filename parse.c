/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chikoh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 20:17:48 by chikoh            #+#    #+#             */
/*   Updated: 2025/08/05 23:24:39 by chikoh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"

void	*parse_command(t_list **list)
{
	t_ast_node	*current;

	current = (t_ast_node *)ft_calloc(sizeof(t_ast_node), 1);
	current->node = (t_token *)(*list)->content;
	while (current->node->type == DOUBLE_QUOTE_STRING
		|| current->node->type == SINGLE_QUOTE_STRING || current->node->REDIRECT_INPUT
		|| current->node->type == HERE_DOC || current->node->type == REDIRECT_OUTPUT
		|| current->node->type == REDIRECT_APPEND || current->node->type == STRING
		|| current->node->type == ASSIGNMENT || current->node->type == COMMAND
		|| current->node->type == ARGUMENTS || current->node->type == WILDCARD
		|| current->node->type == VARIABLE)
	{
		if (current->node->type == REDIRECT_APPEND || current->node->type == REDIRECT_OUTPUT
			|| current->node->type == REDIRECT_INPUT || current->node->type == HERE_DOC)
			ft_lstadd_back(&(node->redirection), (*list)->content);
		else
			ft_lstadd_back(&(node->command), (*list)->content);
		*list = (*list)->next;
	}
	return (current);
}

void	*parse_pipeline(t_list **list)
{
	t_ast_node	*left_node;
	t_ast_node	*right_node;
	t_ast_node	*current;
	t_token	*cur_tok;

	left_node = parse_command(list);
	cur_tok = (t_token *)(*list)->content;
	current = 0;
	while (cur_tok->type == LOGICAL_OR || cur_tok->type == LOGICAL_AND)
	{
		*list = (*list)->next;
		current = (t_ast_node *)ft_calloc(sizeof(t_ast_node), 1);
		current->node = cur_tok;
		right_node = parse_command(list);
		current->left = left_node;
		current->right = right_node;
	}
	return (current);
}

void	*parse_list(t_list **list)
{
	t_ast_node	*left_node;
	t_ast_node	*right_node;
	t_ast_node	*current;
	t_token	*cur_tok;

	left_node = parse_pipeline(list);
	cur_tok = (t_token *)(*list)->content;
	current = 0;
	while (cur_tok->type == LOGICAL_OR || cur_tok->type == LOGICAL_AND)
	{
		*list = (*list)->next;
		current = (t_ast_node *)ft_calloc(sizeof(t_ast_node), 1);
		current->node = cur_tok;
		right_node = parse_pipeline(list);
		current->left = left_node;
		current->right = right_node;
	}
	return (current);
}
