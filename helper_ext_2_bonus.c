/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_ext_2_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 22:58:01 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/31 21:20:00 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

extern int	g_ret_code;

//Add values into myenvp also when its added in default envp
void	ft_add_to_myenvp(t_variable_context *context, t_env *temp_env)
{
	char	*temp_key;
	char	*temp;
	int		exists;

	temp_key = ft_strjoin("declare -x ", temp_env->new_env_key);
	exists = ft_add_to_myenvp_2(temp_env, temp_key, context);
	if (exists == 1)
		return ;
	if (temp_env->new_env_value)
	{
		temp = join_dup_envp(temp_key, temp_env->new_env_value);
		free(temp_key);
		temp_key = temp;
	}
	context->dup_environment_variables
		= ft_copy_envp(context->dup_environment_variables, temp_key);
	free(temp_key);
}

int	ft_update_envp_2(t_env *temp_envp, char *temp_key,
						t_variable_context *context)
{
	int	i;

	i = 0;
	while (context->environment_variables[i])
	{
		if (ft_strncmp(context->environment_variables[i], temp_key,
				ft_strlen(temp_key)) == 0)
		{
			if (temp_envp->new_env_value)
			{
				free(context->environment_variables[i]);
				context->environment_variables[i]
					= ft_strjoin(temp_key, temp_envp->new_env_value);
				ft_add_to_myenvp(context, temp_envp);
				free(temp_key);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

void	ft_update_envp_exp(t_env *temp_envp,
							t_variable_context *context)
{
	int		i;
	char	*temp_key;
	char	*temp;
	int		exists;

	i = 0;
	if (temp_envp->new_env_key && ft_strlen(temp_envp->new_env_key) > 0
		&& ft_isalpha(((char *)temp_envp->new_env_key)[0])
		&& (((char *)temp_envp->new_env_key)[0] != '_'))
	{
		temp_key = ft_strjoin(temp_envp->new_env_key, "=");
		exists = ft_update_envp_2(temp_envp, temp_key, context);
		if (exists == 1)
			return ;
		if (temp_envp->new_env_value)
		{
			temp = ft_strjoin(temp_key, temp_envp->new_env_value);
			free(temp_key);
			temp_key = temp;
			context->environment_variables
				= ft_copy_envp(context->environment_variables, temp_key);
		}
		free(temp_key);
		ft_add_to_myenvp(context, temp_envp);
	}
}

void	ft_update_envp_cd(t_env *temp_envp,
							t_variable_context *context)
{
	int		i;

	i = 0;
	while (context->environment_variables[i])
	{
		if (ft_strncmp(context->environment_variables[i], "PWD=", 4) == 0)
		{
			temp_envp->new_env_key = "PWD";
			temp_envp->new_env_value = temp_envp->pwd;
			free(context->environment_variables[i]);
			context->environment_variables[i]
				= ft_strjoin("PWD=", temp_envp->pwd);
			ft_add_to_myenvp(context, temp_envp);
		}
		else if (!ft_strncmp(context->environment_variables[i], "OLDPWD=", 7))
		{
			temp_envp->new_env_key = "OLDPWD";
			temp_envp->new_env_value = temp_envp->oldpwd;
			free(context->environment_variables[i]);
			context->environment_variables[i]
				= ft_strjoin("OLDPWD=", temp_envp->oldpwd);
			ft_add_to_myenvp(context, temp_envp);
		}
		i++;
	}
}

//To update the entries in envp
void	ft_update_envp(t_env *temp_envp, t_list *command,
						t_variable_context *context)
{
	if (ft_strncmp(command->content, "cd", ft_strlen(command->content)) == 0)
		ft_update_envp_cd(temp_envp, context);
	if (ft_strncmp(command->content, "export",
			ft_strlen(command->content)) == 0)
		ft_update_envp_exp(temp_envp, context);
}
