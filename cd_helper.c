/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 10:57:44 by pchowdry          #+#    #+#             */
/*   Updated: 2025/09/02 11:10:51 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

extern int	g_ret_code;

int		ft_cd_path_len(char *str)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (str[i])
	{
		if (str[i] && str[i] != '\\')
			len++;
		else if (str[i] && str[i + 1]
					&& str[i] == '\\' && str[i + 1] == '\\')
		{
			len++;
			i++;
		}
		i++;
	}
	return (len);
}

char	*ft_cd_path(char *str)
{
	int		i;
	int		j;
	char	*path;
	
	i = 0;
	j = 0;
	path = ft_calloc(ft_cd_path_len(str) + 1, sizeof(char));
	while (str[i])
	{
		if (str[i] && str[i] != '\\')
		{
			path[j] = str[i];
			j++;
		}
		else if (str[i] && str[i + 1]
					&& str[i] == '\\' && str[i + 1] == '\\')
		{
			path[j] = str[i];
			j++;
			i++;
		}
		i++;
	}
	return (path);
}

char	*ft_get_cd_path(t_list *command)
{
	t_list	*temp;

	temp = command;
	if (temp->next)
	{
		temp = temp->next;
		if (ft_strchr(temp->content, '\\'))
			return (ft_cd_path((char *)temp->content));
		else
			return (ft_strdup((char *)temp->content));
	}
	return (NULL);
}

int	ft_cd_error(t_list *command)
{
	char	*remove_backslash;
	int		i;
	int		j;

	i = 0;
	j = 0;
	remove_backslash = ft_get_cd_path(command);
	printf("minishell: cd: %s: %s\n", remove_backslash, strerror(errno));
	free(remove_backslash);
	return (1);
}

void	ft_cd_home(t_env *temp_envp, t_list *command,
					t_variable_context *context)
{
	if (chdir(getenv("HOME")) == 0)
	{
		temp_envp->pwd = getcwd(NULL, 0);
		temp_envp->oldpwd = ft_extract_envp(context->environment_variables,
				"PWD=");
		ft_update_envp(temp_envp, command, context);
	}	
}