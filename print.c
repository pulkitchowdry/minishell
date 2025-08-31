/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:08:08 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/30 20:43:59 by chikoh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

extern int	g_ret_code;

void	print_signal(int signal)
{
	(void)signal;
	rl_on_new_line();
	printf("\n");
	rl_replace_line("", 0);
	rl_redisplay();
}

void	print_new_line(int signal)
{
	(void)signal;
	printf("\n");
}

void	print_string_list(t_list *list)
{
	while (list != 0)
	{
		printf("%s ", (char *)list->content);
		list = list->next;
	}
}

void	print_ast(t_ast_node *node)
{
	if (node == 0)
		return ;
	printf("(");
	print_ast(node->left);
	if (node->node != 0)
		printf("node: %s", node->node->string);
	if (node->command != 0)
	{
		printf("command: ");
		print_string_list(node->command);
	}
	if (node->redirection != 0)
	{
		printf("redirection: ");
		print_string_list(node->redirection);
	}
	if (node->assignment != 0)
	{
		printf("assignment: ");
		print_string_list(node->assignment);
	}
	print_ast(node->right);
	printf(")");
}
