/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 11:57:59 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/04 14:15:27 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	// int j = 0;
	// while (copy[j])
	// {
	// 	write(1, copy[j], ft_strlen(copy[j]));
	// 	write(1, "\n", 1);
	// 	j++;
	// }
	return (copy);
}
//Initially duplicating envp into temp for export command
char	**ft_dup_envp(char **envp)
{
	size_t	envp_len;
	char	**copy;
	int		i;
	t_env	temp;
	int		j;
	
	i = 0;
	envp_len = ft_env_length(envp);
	copy = malloc((envp_len + 1) * sizeof(char *));
	while (envp[i])
	{
		temp.new_env = ft_split(envp[i], '=');
		temp.new_env_key = temp.new_env[0];
		temp.new_env_value = temp.new_env[1];
		copy[i] = ft_strjoin("declare -x ", temp.new_env_key);
		copy[i] = ft_strjoin(copy[i], "=\"");
		copy[i] = ft_strjoin(copy[i], temp.new_env_value);
		j = 2;
		while (temp.new_env[j] != NULL)
		{
			copy[i] = ft_strjoin(copy[i], "=");
			copy[i] = ft_strjoin(copy[i], temp.new_env[j]);
			j++;
		}
		copy[i] = ft_strjoin(copy[i], "\"");
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

//Add values into myenvp also when its added in default envp
void	ft_add_to_myenvp(t_data *data, t_env *temp_env)
{
	int		i;
	char	*temp_key;

	i = 0;
	temp_key = ft_strjoin("declare -x ", temp_env->new_env_key);
	while (data->myenvp[i])
	{
		if (ft_strncmp(data->myenvp[i], temp_key, ft_strlen(temp_key)) == 0)
		{
			if (temp_env->new_env_value)
			{
				data->myenvp[i] = ft_strjoin(temp_key, "=");
				data->myenvp[i] = ft_strjoin(data->myenvp[i], "\"");
				data->myenvp[i] = ft_strjoin(data->myenvp[i], temp_env->new_env_value);
				data->myenvp[i] = ft_strjoin(data->myenvp[i], "\"");
				return ;
			}
			else
			{
				data->myenvp[i] = ft_strdup(temp_key);
				return ;
			}
		}
		i++;
	}
	if (temp_env->new_env_value)
	{
		temp_key = ft_strjoin(temp_key, "=");
		temp_key = ft_strjoin(temp_key, "\"");
		temp_key = ft_strjoin(temp_key, temp_env->new_env_value);
		temp_key = ft_strjoin(temp_key, "\"");
		data->myenvp = ft_copy_envp(data->myenvp, temp_key);
	}
	else
		data->myenvp = ft_copy_envp(data->myenvp, temp_key);
}

//To update the entries in envp
void	ft_update_envp(t_env *temp_envp, t_data *data, char **envp)
{
	int	i;

	i = 0;
	if (ft_strncmp(data->cmd[0], "cd", ft_strlen(data->cmd[0])) == 0)
	{
		while (envp[i])
		{
			if (ft_strncmp(envp[i], "PWD=", 4) == 0)
			{
				temp_envp->new_env_key = "PWD";
				temp_envp->new_env_value = temp_envp->pwd;
				envp[i] = ft_strjoin("PWD=", temp_envp->pwd);
				ft_add_to_myenvp(data, temp_envp);
			}
			else if (ft_strncmp(envp[i], "OLDPWD=", 7) == 0)
			{
				temp_envp->new_env_key = "OLDPWD";
				temp_envp->new_env_value = temp_envp->oldpwd;
				envp[i] = ft_strjoin("OLDPWD=", temp_envp->oldpwd);
				ft_add_to_myenvp(data, temp_envp);
			}
			i++;
		}
	}
	i = 0;//Need to fix quotes in input like export my="val=val=val"
	if (ft_strncmp(data->cmd[0], "export", ft_strlen(data->cmd[0])) == 0)
	{
			while (envp[i])
			{
				if (ft_strncmp(envp[i], temp_envp->new_env_key, ft_strlen(temp_envp->new_env_key)) == 0)
				{
					if (temp_envp->new_env_value)
					{
						envp[i] = ft_strjoin(temp_envp->new_env_key, "=");
						envp[i] = ft_strjoin(envp[i], temp_envp->new_env_value);
						ft_add_to_myenvp(data, temp_envp);
						return ;
					}
				}
				i++;
			}
			if (temp_envp->new_env_value)
			{
				envp[i] = ft_strjoin(temp_envp->new_env_key, "=");
				envp[i] = ft_strjoin(envp[i], temp_envp->new_env_value);
				i++;
				envp[i] = NULL;
			}
			// else
			// {
			// 	data->myenvp = ft_copy_envp(data->myenvp, temp_envp->new_env_key);
			// }
			ft_add_to_myenvp(data, temp_envp);
	}
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
//To execute builtin func, likely to be split into two or more functions later
void	ft_builtin_exec(t_data *data, char **envp)
{
	t_env	temp_envp;
	int		i;
	int		j;
	
	ft_bzero(&temp_envp, sizeof(temp_envp));
	if (ft_strncmp(data->cmd[0], "cd", ft_strlen(data->cmd[0])) == 0)
	{
		if (chdir(data->cmd[1]) == 0)
		{
			temp_envp.pwd = getcwd(NULL, 0);
			temp_envp.oldpwd = ft_extract_envp(envp, "PWD=");
			ft_update_envp(&temp_envp, data, envp);
		}
		else if (!data->cmd[1])
		{
			if (chdir(getenv("HOME")) == 0)
			{
				temp_envp.pwd = getcwd(NULL, 0);
				temp_envp.oldpwd = ft_extract_envp(envp, "PWD=");
				ft_update_envp(&temp_envp, data, envp);
			}
		}
		else
			perror("cd");
	}
	if (ft_strncmp(data->cmd[0], "export", ft_strlen(data->cmd[0])) == 0)
	{
		if (data->cmd[1])
		{
			//Need to find for the variable and update or add it
			temp_envp.new_env = ft_split(data->cmd[1], '=');
			temp_envp.new_env_key = temp_envp.new_env[0];
			if (temp_envp.new_env[1])
				temp_envp.new_env_value = temp_envp.new_env[1];
			j = 2;
			while (temp_envp.new_env[j] != NULL)
			{
				temp_envp.new_env_value = ft_strjoin(temp_envp.new_env_value, "=");
				temp_envp.new_env_value = ft_strjoin(temp_envp.new_env_value, temp_envp.new_env[j]);
				j++;
			}
			ft_update_envp(&temp_envp, data, envp);
		}
		else if (!data->cmd[1])
		{
			//Need to display environment variables with declare -x infront
			//And values within quotes
			i = 0;
			while (data->myenvp[i])
			{
				write(1, data->myenvp[i], ft_strlen(data->myenvp[i]));
				write(1, "\n", 1);
				i++;
			}
			return ;
		}
	}
}

// To check if the command is a builtin command or not
//echo, pwd, env work through execve also because there are external func
//Removed echo, pwd and env as they work fine with execve function
int	is_builtin(char *str)
{
	if (str && (ft_strncmp(str, "cd", ft_strlen(str)) == 0
		|| ft_strncmp(str, "export", ft_strlen(str)) == 0
		|| ft_strncmp(str, "unset", ft_strlen(str)) == 0
		|| ft_strncmp(str, "exit", ft_strlen(str)) == 0))
		return (1);
	return (0);
}