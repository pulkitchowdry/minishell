/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_ext_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 22:56:34 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/31 15:21:48 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

extern int	g_ret_code;

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
	size_t	equal_pos;

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
	return (result);
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
	return (result);
}

char	*join_dup_envp(char *key, char *value)
{
	char	*join;
	char	*temp;

	join = ft_strjoin(key, "=");
	temp = ft_strjoin(join, "\"");
	free(join);
	join = temp;
	temp = ft_strjoin(join, value);
	free(join);
	join = temp;
	temp = ft_strjoin(join, "\"");
	free(join);
	join = temp;
	return (join);
}

int	ft_add_to_myenvp_2(t_env *temp_env, char *temp_key,
						t_variable_context *context)
{
	int		i;
	char	*extract;

	i = 0;
	while (context->dup_environment_variables[i])
	{
		extract = ft_get_key(context->dup_environment_variables[i]);
		if (ft_strncmp(extract, temp_key, ft_strlen(extract) + 1) == 0)
		{
			free(context->dup_environment_variables[i]);
			if (temp_env->new_env_value)
				context->dup_environment_variables[i]
					= join_dup_envp(temp_key, temp_env->new_env_value);
			else
				context->dup_environment_variables[i] = ft_strdup(temp_key);
			free(temp_key);
			free(extract);
			return (1);
		}
		free(extract);
		i++;
	}
	return (0);
}
