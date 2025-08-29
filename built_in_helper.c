/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:13:21 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/29 17:48:15 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_ret_code;
//To get length of the envp array
size_t	ft_env_length(char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
		i++;
	return(i);
}
//Adding a new key in temp whenever there is no value
char	**ft_copy_envp(char **myenvp, char *new)
{
	size_t	envp_len;
	char	**copy;
	int		i;
	
	i = 0;
	envp_len = ft_env_length(myenvp);
	copy = malloc((envp_len + 2) * sizeof(char *));
	while (myenvp[i])
	{
		copy[i] = ft_strdup(myenvp[i]);
		i++;
	}
	copy[i] = ft_strdup(new);
	i++;
	copy[i] = NULL;
	return (copy);
}

char	**ft_dup_str_array(char **envp)
{
	size_t	envp_len;
	char	**copy;
	int		i;
	
	i = 0;
	envp_len = ft_env_length(envp);
	copy = malloc((envp_len + 1) * sizeof(char *));
	while (envp[i])
	{
		copy[i] = ft_strdup(envp[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}
char	*ft_get_var(char *temp_key, char *temp_value)
{
	char	*str;
	char	*temp;

	str = ft_strjoin("declare -x ", temp_key);
	if (temp_value)
	{
		temp = ft_strjoin(str, "=\"");
		free(str);
		str = temp;
		temp = ft_strjoin(str, temp_value);
		free(str);
		str = temp;
		temp = ft_strjoin(str, "\"");
		free(str);
		str = temp;
	}
	return(temp);
}

//Initially duplicating envp into temp for export command
char	**ft_dup_envp(char **envp)
{
	char	**copy;
	char	*temp_key;
	char	*temp_value;
	int		i;
	
	copy = ft_calloc(sizeof(char *), ft_size(envp) + 1);
	i = 0;
	while (envp[i])
	{
		temp_key = ft_get_key(envp[i]);
		temp_value = ft_get_value(envp[i]);
		copy[i] = ft_get_var(temp_key, temp_value);
		free(temp_key);
		free(temp_value);
		i++;
	}
	return (copy);
}

int	ft_find_equal(char *str)
{
	int	i;
	
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	return (i);
}

char	*ft_get_value(char *str)
{
	int		i;
	char	*result;
	int		equal_pos;
	
	i = 0;
	equal_pos = ft_find_equal(str);
	if (ft_strlen(str) - equal_pos == 0)
		return (NULL);
	result = ft_calloc(sizeof(char), ft_strlen(str) - equal_pos);
	equal_pos++;
	while (equal_pos < ft_strlen(str))
	{
		result[i] = str[equal_pos];
		equal_pos++;
		i++;
	}
	return(result);
}

char	*ft_get_key(char *str)
{
	int		i;
	char	*result;
	int		equal_pos;
	
	i = 0;
	equal_pos = ft_find_equal(str);
	if (equal_pos == 0)
		return (NULL);
	result = ft_calloc(sizeof(char), equal_pos + 1);
	while (i < equal_pos)
	{
		result[i] = str[i];
		i++;
	}
	return(result);
}

char	*join_dup_envp(char *key, char *value)
{
	char	*join;
	
	join = ft_strjoin(key, "=");
	join = ft_strjoin(join, "\"");
	join = ft_strjoin(join, value);
	join = ft_strjoin(join, "\"");
	return (join);
}

//Add values into myenvp also when its added in default envp
void	ft_add_to_myenvp(t_variable_context *context, t_env *temp_env)
{
	int		i;
	char	*temp_key;
	char	*extract;

	i = 0;
	temp_key = ft_strjoin("declare -x ", temp_env->new_env_key);
	while (context->dup_environment_variables[i])
	{
		extract = ft_get_key(context->dup_environment_variables[i]);
		if (ft_strncmp(extract, temp_key, ft_strlen(extract) + 1) == 0)
		{
			if (temp_env->new_env_value)
				context->dup_environment_variables[i] = join_dup_envp(temp_key, temp_env->new_env_value);
			else
				context->dup_environment_variables[i] = ft_strdup(temp_key);
			free(temp_key);
			return ;
		}
		free(extract);
		i++;
	}
	if (temp_env->new_env_value)
		temp_key = join_dup_envp(temp_key, temp_env->new_env_value);
	context->dup_environment_variables = ft_copy_envp(context->dup_environment_variables, temp_key);
	free(temp_key);
}

int	ft_update_envp_2(t_env *temp_envp, char *temp_key, t_variable_context *context)
{
	int	i;

	i = 0;
	while (context->environment_variables[i])
	{
		if (ft_strncmp(context->environment_variables[i], temp_key, ft_strlen(temp_key)) == 0)
		{
			if (temp_envp->new_env_value)
			{
				context->environment_variables[i] = ft_strjoin(temp_key, temp_envp->new_env_value);
				ft_add_to_myenvp(context, temp_envp);
				free(temp_key);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

void	ft_update_envp_exp(t_env *temp_envp, t_list *command, t_variable_context *context)
{
	int		i;
	char	*temp_key;
	int		exists;

	i = 0;
	temp_key = ft_strjoin(temp_envp->new_env_key, "=");
	exists = ft_update_envp_2(temp_envp, temp_key, context);
	if (exists == 1)
		return ;
	if (temp_envp->new_env_value)
	{
		temp_key = ft_strjoin(temp_key, temp_envp->new_env_value);
		context->environment_variables = ft_copy_envp(context->environment_variables, temp_key);
	}
	free(temp_key);
	ft_add_to_myenvp(context, temp_envp);
}

void	ft_update_envp_cd(t_env *temp_envp, t_list *command, t_variable_context *context)
{
	int		i;

	i = 0;
	while (context->environment_variables[i])
	{
		if (ft_strncmp(context->environment_variables[i], "PWD=", 4) == 0)
		{
			temp_envp->new_env_key = "PWD";
			temp_envp->new_env_value = temp_envp->pwd;
			context->environment_variables[i] = ft_strjoin("PWD=", temp_envp->pwd);
			ft_add_to_myenvp(context, temp_envp);
		}
		else if (ft_strncmp(context->environment_variables[i], "OLDPWD=", 7) == 0)
		{
			temp_envp->new_env_key = "OLDPWD";
			temp_envp->new_env_value = temp_envp->oldpwd;
			context->environment_variables[i] = ft_strjoin("OLDPWD=", temp_envp->oldpwd);
			ft_add_to_myenvp(context, temp_envp);
		}
		i++;
	}
}

//To update the entries in envp
void	ft_update_envp(t_env *temp_envp, t_list *command, t_variable_context *context)
{
	if (ft_strncmp(command->content, "cd", ft_strlen(command->content)) == 0)
		ft_update_envp_cd(temp_envp, command, context);
	if (ft_strncmp(command->content, "export", ft_strlen(command->content)) == 0)
		ft_update_envp_exp(temp_envp, command, context);
}

//To extract variable from envp
char	*ft_extract_envp(char **envp, char *str)
{
	char	*value;
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], str, ft_strlen(str)) == 0)
			value = envp[i] + ft_strlen(str);
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
	new = ft_calloc(sizeof(char *), ft_env_length(context->environment_variables) + 1);
	while (context->environment_variables[i])
	{
		extract = ft_get_key(context->environment_variables[i]);
		if (ft_strncmp(extract, temp_envp->char_unset, ft_strlen(extract) + 1) != 0)
		{
			new[j] = context->environment_variables[i];
			j++;
		}
		i++;
	}
	free(extract);
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
	new = ft_calloc(sizeof(char *), ft_env_length(context->dup_environment_variables) + 1);
	while (context->dup_environment_variables[i])
	{
		extract = ft_get_key(context->dup_environment_variables[i]);
		if (ft_strncmp(extract, temp_envp->char_unset, ft_strlen(extract) + 1) != 0)
		{
			new[j] = context->dup_environment_variables[i];
			j++;
		}
		i++;
	}
	free(extract);
	return (new);
}

char	**ft_remove_from_local(t_variable_context *context, t_env *temp_envp)
{
	int			i;
	int			j;
	char	**new;
	char	*extract;

	i = 0;
	j = 0;
	new = ft_calloc(sizeof(char *), ft_env_length(context->local_variables) + 1);
	while (context->local_variables[i])
	{		
		extract = ft_get_key(context->dup_environment_variables[i]);
		if (ft_strncmp(extract, temp_envp->char_unset, ft_strlen(extract) + 1) != 0)
		{
			new[j] = context->local_variables[i];
			j++;
		}
		i++;
	}
	free(extract);
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
	while (context->dup_environment_variables && context->dup_environment_variables[i])
	{
		if (ft_strncmp(copy, context->dup_environment_variables[i], ft_strlen(copy) + 1) == 0)
			ft_update_envp(&temp_env, command, context);
		i++;
	}
	free(command);
	free(temp_env.new_env_key);
	free(copy);
	free(temp_env.new_env_value);
}
