/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:13:21 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/29 23:23:45 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_ret_code;

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
	free_string_array(myenvp);
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
	return (str);
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

char	*ft_oldpwd(char *str, size_t start)
{
	char	*result;
	size_t	i;

	i = 0;
	result = ft_calloc(sizeof(char), ft_strlen(str) - start + 1);
	while (str[start])
	{
		result[i] = str[start];
		i++;
		start++;
	}
	return (result);
}
