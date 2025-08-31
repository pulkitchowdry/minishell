/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:08:31 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/31 15:21:52 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>

#include "libft/libft.h"
#include "minishell_bonus.h"

int		g_ret_code = 0;

void	initialize(char **envp, t_variable_context *context,
					t_state_context *state_context)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, print_signal);
	state_context->context = context;
	state_context->current = 0;
	context->environment_variables = ft_dup_str_array(envp);
	context->local_variables = 0;
	context->dup_environment_variables = ft_dup_envp(envp);
}

void	free_no_ip(t_variable_context *context)
{
	rl_clear_history();
	free_string_array(context->environment_variables);
	free_string_array(context->dup_environment_variables);
	free_string_array(context->local_variables);
	close(0);
	close(1);
	close(2);
}

void	parse_and_exec(char	*input, t_state_context *state_context)
{
	t_list		*list_start;
	t_list		*list;
	t_ast_node	*root;

	add_history(input);
	list = create_tokens(input);
	list_start = list;
	root = parse_list(&list, state_context);
	state_context->current = root;
	if (root != 0 && is_safe_to_execute(root) && list == 0)
	{
		g_ret_code = execute_heredoc(root, list_start, state_context, 0);
		if (g_ret_code == 0)
			g_ret_code = execute_command_ast(root, list_start,
					root, state_context->context);
	}
	else
	{
		printf("Invalid command\n");
		g_ret_code = 1;
	}
	unlink_files(root);
	free_command(&root);
	ft_lstclear(&list_start, free_token);
	free(input);
}

int	main(int argc, char **argv, char **envp)
{
	char				*input;
	t_variable_context	context;
	t_state_context		state_context;

	(void)argc;
	(void)argv;
	initialize(envp, &context, &state_context);
	while (1)
	{
		input = readline("->");
		if (input && ft_strncmp(input, "", 1) != 0)
			parse_and_exec(input, &state_context);
		else if (input == 0)
			break ;
	}
	free_no_ip(&context);
	return (0);
}
