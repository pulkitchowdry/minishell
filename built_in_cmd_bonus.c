/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmd_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:11:34 by pchowdry          #+#    #+#             */
/*   Updated: 2025/09/02 13:53:52 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

extern int	g_ret_code;

int	ft_echo(t_list *command)
{
	t_list	*t_cmd;
	t_env	temp_envp;

	t_cmd = command;
	ft_bzero(&temp_envp, sizeof(temp_envp));
	if (t_cmd->next)
	{
		t_cmd = t_cmd->next;
		if (ft_strlen(t_cmd->content) > 0
			&& ft_strncmp(t_cmd->content, "-n", ft_strlen(t_cmd->content)) == 0)
		{
			temp_envp.no_new_line_flag = 1;
			write(1, "", 0);
			t_cmd = t_cmd->next;
		}
		while (t_cmd)
		{
			write(1, t_cmd->content, ft_strlen(t_cmd->content));
			if (t_cmd->next)
				write(1, " ", 1);
			t_cmd = t_cmd->next;
		}
	}
	ft_echo_new_line(&temp_envp);
	return (0);
}

void	ft_unset_dup(t_env *temp_envp, t_variable_context *context)
{
	int		i;
	char	*extract;
	char	*temp;

	i = 0;
	temp = ft_strjoin("declare -x ", temp_envp->char_unset);
	free(temp_envp->char_unset);
	temp_envp->char_unset = temp;
	while (context->dup_environment_variables
		&& context->dup_environment_variables[i])
	{
		extract = ft_get_key(context->dup_environment_variables[i]);
		if (ft_strncmp(extract, temp_envp->char_unset,
				ft_strlen(extract) + 1) == 0)
			context->dup_environment_variables
				= ft_remove_from_myenvp(context, temp_envp);
		free(extract);
		i++;
	}
}

void	ft_unset_env(t_env *temp_envp, t_variable_context *context)
{
	int		i;
	char	*extract;

	i = 0;
	while (context->environment_variables && context->environment_variables[i])
	{
		extract = ft_get_key(context->environment_variables[i]);
		if (ft_strncmp(extract, temp_envp->char_unset,
				ft_strlen(extract) + 1) == 0)
			context->environment_variables
				= ft_remove_from_dup_envp(context, temp_envp);
		free(extract);
		i++;
	}
	i = 0;
	while (context->local_variables && context->local_variables[i])
	{
		extract = ft_get_key(context->local_variables[i]);
		if (ft_strncmp(extract, temp_envp->char_unset,
				ft_strlen(extract) + 1) == 0)
			context->local_variables = ft_remove_from_local(context, temp_envp);
		free(extract);
		i++;
	}
}

int	ft_unset(t_list *command,
					t_variable_context *context)
{
	t_list	*temp_cmd;
	t_env	temp_envp;

	temp_cmd = command;
	ft_bzero(&temp_envp, sizeof(temp_envp));
	if (temp_cmd->next)
	{
		temp_cmd = temp_cmd->next;
		while (temp_cmd)
		{
			temp_envp.char_unset = ft_strdup(temp_cmd->content);
			ft_unset_env(&temp_envp, context);
			ft_unset_dup(&temp_envp, context);
			free(temp_envp.char_unset);
			temp_cmd = temp_cmd->next;
		}
	}
	return (0);
}

int	ft_env(t_variable_context *context)
{
	int	i;

	i = 0;
	while (context->environment_variables[i])
	{
		write(1, context->environment_variables[i],
			ft_strlen(context->environment_variables[i]));
		write(1, "\n", 1);
		i++;
	}
	return (0);
}
