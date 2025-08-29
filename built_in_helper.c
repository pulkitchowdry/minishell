/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/08/29 14:58:00 by pchowdry         ###   ########.fr       */
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
		copy[i] = ft_strjoin("declare -x ", temp_key);
		free(temp_key);
		if (temp_value)
		{
			copy[i] = ft_strjoin(copy[i], "=\"");
			copy[i] = ft_strjoin(copy[i], temp_value);
			copy[i] = ft_strjoin(copy[i], "\"");
		}
		free(temp_value);
		i++;
	}
	return (copy);
}
// char	**ft_dup_envp(char **envp)
// {
// 	size_t	envp_len;
// 	char	**copy;
// 	int		i;
// 	t_env	temp;
// 	int		j;
// 	char	*temp_string;
	
// 	i = 0;
// 	envp_len = ft_env_length(envp);
// 	ft_bzero(&temp, sizeof(temp));
// 	copy = malloc((envp_len + 1) * sizeof(char *));
// 	while (envp[i])
// 	{
// 		temp.new_env = ft_split(envp[i], '=');
// 		temp.new_env_key = ft_strdup(temp.new_env[0]);
// 		copy[i] = ft_strjoin("declare -x ", temp.new_env_key);
// 		if (temp.new_env[1])
// 			temp.new_env_value = ft_strdup(temp.new_env[1]);
// 		temp_string = ft_strjoin(copy[i], "=\"");
// 		free(copy[i]);
// 		copy[i] = temp_string;
// 		temp_string = ft_strjoin(copy[i], temp.new_env_value);
// 		free(copy[i]);
// 		copy[i] = temp_string;
// 		j = 2;
// 		while (temp.new_env[j] != NULL)
// 		{
// 			temp_string = ft_strjoin(copy[i], "=");
// 			free(copy[i]);
// 			copy[i] = temp_string;
// 			temp_string = ft_strjoin(copy[i], temp.new_env[j]);
// 			free(copy[i]);
// 			copy[i] = temp_string;
// 			j++;
// 		}
// 		temp_string = ft_strjoin(copy[i], "\"");
// 		free(copy[i]);
// 		copy[i] = temp_string;
// 		free_string_array(temp.new_env);
// 		i++;
// 	}
// 	copy[i] = NULL;
// 	return (copy);
// }

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

void	ft_update_envp_exp(t_env *temp_envp, t_list *command, t_variable_context *context)
{
	int		i;
	char	*temp_key;

	i = 0;
	temp_key = ft_strjoin(temp_envp->new_env_key, "=");
	while (context->environment_variables[i])
	{
		if (ft_strncmp(context->environment_variables[i], temp_key, ft_strlen(temp_key)) == 0)
		{
			if (temp_envp->new_env_value)
			{
				context->environment_variables[i] = ft_strjoin(temp_key, temp_envp->new_env_value);
				ft_add_to_myenvp(context, temp_envp);
				free(temp_key);
				return ;
			}
		}
		i++;
	}
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

// t_list	*ft_split_command(t_list *command)
// {
// 	t_list	*split_command;
// 	int		i;

// 	i = 0;
// 	split_command = ft_calloc(sizeof(t_list), ft_lstsize(command));
// 	while (command)
// 	{
// 		if (i = 0)
// 			command = command->next;
// 		else
// 		{
// 			split_command->content = ft_split(command->content, '=')
// 			command = command->next;
// 		}	
// 	}
// }

// char	**ft_add_to_local(t_env *temp_env, t_variable_context *context)
// {
// 	int		i;
// 	char	**new_arr;
// 	char	*key_value;
	
// 	i = 0;
// 	new_arr = ft_calloc(sizeof(char *), ft_size(context->local_variables) + 2);
// 	if (temp_env->new_env_key)
// 	{
// 		key_value = ft_strjoin(temp_env->new_env_key, "=");
// 		while (context->local_variables && context->local_variables[i])
// 		{
// 			if (ft_strncmp(key_value, context->local_variables[i], ft_strlen(key_value)) != 0)
// 				new_arr[i] = context->local_variables[i];
// 			else if (ft_strncmp(key_value, context->local_variables[i], ft_strlen(key_value)) == 0
// 					&& !temp_env->new_env_value)
// 				new_arr[i] = context->local_variables[i];
// 			i++;
// 		}
// 		if (temp_env->new_env_value)
// 		{
// 			printf("ENtered: %s\n", temp_env->new_env_value);
// 			key_value = ft_strjoin(key_value, temp_env->new_env_value);
// 			new_arr[i] = key_value;
// 		}
// 		free(key_value);
// 		return(new_arr);
// 	}
// 	else
// 		return (context->local_variables);
// }

// //To execute builtin func, likely to be split into two or more functions later
// void	ft_builtin_exec(t_data *data, char **envp)
// {
// 	t_env	temp_envp;
// 	int		i;
// 	int		j;
	
// 	ft_bzero(&temp_envp, sizeof(temp_envp));
// 	if (ft_strncmp(data->cmd[0], "cd", ft_strlen(data->cmd[0])) == 0)
// 	{
// 		if (chdir(data->cmd[1]) == 0)
// 		{
// 			temp_envp.pwd = getcwd(NULL, 0);
// 			temp_envp.oldpwd = ft_extract_envp(envp, "PWD=");
// 			ft_update_envp(&temp_envp, data, envp);
// 		}
// 		else if (!data->cmd[1])
// 		{
// 			if (chdir(getenv("HOME")) == 0)
// 			{
// 				temp_envp.pwd = getcwd(NULL, 0);
// 				temp_envp.oldpwd = ft_extract_envp(envp, "PWD=");
// 				ft_update_envp(&temp_envp, data, envp);
// 			}
// 		}
// 		else
// 			perror("cd");
// 	}
// 	if (ft_strncmp(data->cmd[0], "export", ft_strlen(data->cmd[0])) == 0)
// 	{
// 		if (data->cmd[1])
// 		{
// 			//Need to find for the variable and update or add it
// 			temp_envp.new_env = ft_split(data->cmd[1], '=');
// 			temp_envp.new_env_key = temp_envp.new_env[0];
// 			if (temp_envp.new_env[1])
// 				temp_envp.new_env_value = temp_envp.new_env[1];
// 			j = 2;
// 			while (temp_envp.new_env[j] != NULL)
// 			{
// 				temp_envp.new_env_value = ft_strjoin(temp_envp.new_env_value, "=");
// 				temp_envp.new_env_value = ft_strjoin(temp_envp.new_env_value, temp_envp.new_env[j]);
// 				j++;
// 			}
// 			ft_update_envp(&temp_envp, data, envp);
// 		}
// 		else if (!data->cmd[1])
// 		{
// 			//Need to display environment variables with declare -x infront
// 			//And values within quotes
// 			i = 0;
// 			while (data->myenvp[i])
// 			{
// 				write(1, data->myenvp[i], ft_strlen(data->myenvp[i]));
// 				write(1, "\n", 1);
// 				i++;
// 			}
// 			return ;
// 		}
// 	}
// 	i = 0;
// 	if (ft_strncmp(data->cmd[0], "unset", ft_strlen(data->cmd[0])) == 0)
// 	{
// 		if (data->cmd[1])
// 		{
// 			temp_envp.char_unset = ft_strjoin(data->cmd[1], "=");
// 			while (envp[i])
// 			{
// 				if (ft_strncmp(envp[i], temp_envp.char_unset, ft_strlen(temp_envp.char_unset)) == 0)
// 					envp[i] = NULL;
// 				i++;
// 			}
// 			i = 0;
// 			temp_envp.char_unset = ft_strdup(data->cmd[1]);
// 			temp_envp.char_unset = ft_strjoin("declare -x ",temp_envp.char_unset);
// 			temp_envp.char_unset = ft_strjoin(temp_envp.char_unset, "=");
// 			while (data->myenvp[i])
// 			{
// 				if (ft_strncmp(data->myenvp[i], temp_envp.char_unset, ft_strlen(temp_envp.char_unset)) == 0)
// 				{
// 					data->myenvp = ft_remove_from_myenvp(data, &temp_envp);	
// 				}
// 				i++;
// 			}
// 		}
// 	}
// }
//Following is duplicate of an existing function in execute.c
// // To check if the command is a builtin command or not
// //echo, pwd, env work through execve also because there are external func
// //Removed echo, pwd and env as they work fine with execve function
// int	is_builtin(char *str)
// {
// 	if (str && (ft_strncmp(str, "cd", ft_strlen(str)) == 0
// 		|| ft_strncmp(str, "export", ft_strlen(str)) == 0
// 		|| ft_strncmp(str, "unset", ft_strlen(str)) == 0
// 		|| ft_strncmp(str, "exit", ft_strlen(str)) == 0))
// 		return (1);
// 	return (0);
// }
