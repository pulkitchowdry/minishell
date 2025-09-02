/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 22:55:31 by pchowdry          #+#    #+#             */
/*   Updated: 2025/09/02 14:47:50 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

extern int	g_ret_code;

//To get length of the envp array
size_t	ft_env_length(char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

int	ft_exit(t_ast_node *root,
		t_list *token, t_ast_node *node,
		t_variable_context *context)
{
	if (node->redirection)
	{
		close(1023);
		close(1022);
	}
	rl_clear_history();
	free_command(&root);
	unlink_files(root);
	ft_lstclear(&token, free_token);
	free_string_array(context->environment_variables);
	free_string_array(context->dup_environment_variables);
	free_string_array(context->local_variables);
	ft_putstr_fd("exit\n", 2);
	close(0);
	close(1);
	close(2);
	exit(g_ret_code);
	return (0);
}

int	ft_export_2_error(char *str)
{
	printf("export: `%s': not a valid identifier\n", str);
	return (1);
}

void	ft_echo_new_line(t_env *temp_envp)
{
	if (temp_envp->no_new_line_flag != 1)
		write(1, "\n", 1);
}

int	ft_asgmt(char *str, int type)
{
	char	*key;
	char	*value;
	int		valid;

	valid = 0;
	key = ft_get_key(str);
	value = ft_get_value(str);
	if (type == 1 && key)
		valid = 1;
	else if (type == 2 && value)
		valid = 1;
	else
		valid = 0;
	free(key);
	free(value);
	return (valid);
}
