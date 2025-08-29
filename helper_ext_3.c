/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_ext_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 22:59:15 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/29 23:21:58 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

extern int	g_ret_code;

//To extract variable from envp
char	*ft_extract_envp(char **envp, char *str)
{
	char	*value;
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], str, ft_strlen(str)) == 0)
			value = ft_oldpwd(envp[i], ft_strlen(str));
		i++;
	}
	return (value);
}

char	**ft_remove_from_dup_envp(t_variable_context *context, t_env *temp_envp)
{
	int		i;
	int		j;
	char	**new;
	char	*extract;

	i = 0;
	j = 0;
	new = ft_calloc(sizeof(char *),
			ft_env_length(context->environment_variables) + 1);
	while (context->environment_variables[i])
	{
		extract = ft_get_key(context->environment_variables[i]);
		if (ft_strncmp(extract, temp_envp->char_unset,
				ft_strlen(extract) + 1) != 0)
		{
			new[j] = ft_strdup(context->environment_variables[i]);
			j++;
		}
		free(extract);
		i++;
	}
	free_string_array(context->environment_variables);
	return (new);
}

char	**ft_remove_from_myenvp(t_variable_context *context, t_env *temp_envp)
{
	int		i;
	int		j;
	char	**new;
	char	*extract;

	i = 0;
	j = 0;
	new = ft_calloc(sizeof(char *),
			ft_env_length(context->dup_environment_variables) + 1);
	while (context->dup_environment_variables[i])
	{
		extract = ft_get_key(context->dup_environment_variables[i]);
		if (ft_strncmp(extract, temp_envp->char_unset,
				ft_strlen(extract) + 1) != 0)
		{
			new[j] = ft_strdup(context->dup_environment_variables[i]);
			j++;
		}
		free(extract);
		i++;
	}
	free_string_array(context->dup_environment_variables);
	return (new);
}

char	**ft_remove_from_local(t_variable_context *context, t_env *temp_envp)
{
	int		i;
	int		j;
	char	**new;
	char	*extract;

	i = 0;
	j = 0;
	new = ft_calloc(sizeof(char *),
			ft_env_length(context->local_variables) + 1);
	while (context->local_variables[i])
	{
		extract = ft_get_key(context->local_variables[i]);
		if (ft_strncmp(extract, temp_envp->char_unset,
				ft_strlen(extract) + 1) != 0)
		{
			new[j] = ft_strdup(context->local_variables[i]);
			j++;
		}
		free(extract);
		i++;
	}
	free_string_array(context->local_variables);
	return (new);
}

void	ft_add_to_others(char *str, t_variable_context *context)
{
	t_env	temp_env;
	t_list	*command;
	char	*copy;
	int		i;

	i = 0;
	command = ft_calloc(sizeof(t_list), 1);
	command->content = "export";
	ft_bzero(&temp_env, sizeof(temp_env));
	temp_env.new_env_key = ft_get_key(str);
	copy = ft_strjoin("declare -x ", temp_env.new_env_key);
	temp_env.new_env_value = ft_get_value(str);
	while (context->dup_environment_variables
		&& context->dup_environment_variables[i])
	{
		if (ft_strncmp(copy, context->dup_environment_variables[i],
				ft_strlen(copy) + 1) == 0)
			ft_update_envp(&temp_env, command, context);
		i++;
	}
	free(command);
	free(temp_env.new_env_key);
	free(copy);
	free(temp_env.new_env_value);
}
