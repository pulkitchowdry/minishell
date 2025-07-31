/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:08:31 by pchowdry          #+#    #+#             */
/*   Updated: 2025/07/29 22:35:51 by chikoh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>

void	print_signal(int signal)
{
	rl_on_new_line();
	printf("\n");
	rl_redisplay();
}

#include "minishell.h"

void	ft_error(void)
{
	write(1, strerror(errno), ft_strlen(strerror(errno)));
	write(1, "\n", 1);
}

void	ft_minishell(t_data *data, char **envp)
{
	data->c_id = fork();
	if (data->c_id == 0)
	{
		execve(data->cmd_path, data->cmd, envp);
	}
	waitpid(data->c_id, NULL, 0);
}

char	*ft_find_path(char **envp)
{
	char *path;
	int		i;

	i = 0;
	while (envp[i])
	{
		if (envp[i][0] == 'P' && envp[i][1] == 'A'
			&& envp[i][2] == 'T' && envp[i][3] == 'H')
			path = envp[i] + 4;
		i++;
	}
	if (!path)
		path = "/usr/bin:/bin";
	return (path);	
}

char	*ft_cmd_path(t_data *data)
{
	int 	i;
	char	*cmd_path;

	i = 0;
	while (data->path_dir[i])
	{
		data->p_temp = ft_strjoin(data->path_dir[i], "/");
		cmd_path = ft_strjoin(data->p_temp, data->cmd[0]);
		if (access(cmd_path, X_OK) == 0)
			return (cmd_path);
		i++;
	}
	return (NULL);
}

int	main(int argc, char **argv, char **envp)
{
	char	*string_cmd;

	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, print_signal);
	t_data	data;

	if(argc > 0 && argv[0])
	{
		while (1)
		{
			data.input = readline("->");
			if (data.input)
			{
				add_history(data.input);
			}
			data.cmd = ft_split(data.input, ' ');
			data.path = ft_find_path(envp);
			data.path_dir = ft_split(data.path, ':');
			data.cmd_path = ft_cmd_path(&data);
			if (data.cmd_path)
				ft_minishell(&data, envp);
			else
				ft_error();
		}
	}
	return (0);
}