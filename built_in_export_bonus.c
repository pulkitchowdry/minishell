/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 22:53:23 by pchowdry          #+#    #+#             */
/*   Updated: 2025/09/02 11:21:55 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

extern int	g_ret_code;

int	ft_cd(t_list *command, t_variable_context *context)
{
	t_list	*temp_cmd;
	t_env	temp_envp;
	int		ret_code;

	ret_code = 0;
	temp_cmd = command;
	ft_bzero(&temp_envp, sizeof(temp_envp));
	if (temp_cmd->next && chdir(temp_cmd->next->content) == 0)
	{
		temp_envp.pwd = getcwd(NULL, 0);
		temp_envp.oldpwd = ft_extract_envp(context->environment_variables,
				"PWD=");
		ft_update_envp(&temp_envp, command, context);
	}
	else if (!temp_cmd->next
		|| (temp_cmd->next && ft_strncmp(temp_cmd->next->content, "~", 2) == 0))
		ft_cd_home(&temp_envp, command, context);
	else
		ret_code = ft_cd_error(command);
	free(temp_envp.pwd);
	free(temp_envp.oldpwd);
	return (ret_code);
}

int	ft_pwd(void)
{
	char	*string;

	string = getcwd(NULL, 0);
	if (string)
		printf("%s\n", string);
	else
		ft_putstr_fd("pwd error", 2);
	free(string);
	return (0);
}

void	ft_export_3(t_env *temp_envp, t_variable_context *context)
{
	int	i;

	i = 0;
	while (context->local_variables && context->local_variables[i] != NULL)
	{
		temp_envp->local_key = ft_get_key(context->local_variables[i]);
		temp_envp->local_value = ft_get_value(context->local_variables[i]);
		if (temp_envp->local_key
			&& ft_strncmp(temp_envp->new_env_key, temp_envp->local_key,
				ft_strlen(temp_envp->local_key) + 1) == 0
			&& ft_strlen(temp_envp->local_key))
		{
			if (temp_envp->local_value)
				temp_envp->new_env_value = ft_strdup(temp_envp->local_value);
		}
		free(temp_envp->local_key);
		free(temp_envp->local_value);
		i++;
	}
}

int	ft_export_2(t_list *command, t_list *redirection,
						t_variable_context *context)
{
	t_list	*temp_cmd;
	t_env	temp_envp;
	int		flag;

	flag = 0;
	temp_cmd = command;
	ft_bzero(&temp_envp, sizeof(temp_envp));
	temp_cmd = temp_cmd->next;
	while (temp_cmd)
	{
		if (ft_strncmp(temp_cmd->content, "=", 2) == 0
			|| !ft_isalpha(((char *)temp_cmd->content)[0])
			|| (((char *)temp_cmd->content)[0] == '_'))
			flag = ft_export_2_error((char *)temp_cmd->content);
		temp_envp.new_env_key = ft_get_key(temp_cmd->content);
		temp_envp.new_env_value = ft_get_value(temp_cmd->content);
		if (!temp_envp.new_env_value)
			ft_export_3(&temp_envp, context);
		append_local_variables_2(command, redirection, context);
		ft_update_envp(&temp_envp, command, context);
		free(temp_envp.new_env_key);
		free(temp_envp.new_env_value);
		temp_cmd = temp_cmd->next;
	}
	return (flag);
}

int	ft_export(t_list *command, t_list *redirection,
					t_variable_context *context)
{
	int	i;

	i = 0;
	if (command->next)
		return (ft_export_2(command, redirection, context));
	else if (!command->next)
	{
		while (context->dup_environment_variables[i])
		{
			write(1, context->dup_environment_variables[i],
				ft_strlen(context->dup_environment_variables[i]));
			write(1, "\n", 1);
			i++;
		}
	}
	return (0);
}
