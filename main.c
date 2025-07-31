/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:08:31 by pchowdry          #+#    #+#             */
/*   Updated: 2025/07/31 15:10:10 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_error(void)
{
	write(1, strerror(errno), ft_strlen(strerror(errno)));
	write(1, "\n", 1);
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

void	ft_child_minishell(t_data *data, char **envp, int i)
{
	if (i == 0)
	{
		dup2(data->pipe_fd[1], STDOUT_FILENO);
	}
	else if (i > 0 && i < data->pipes)
	{
		dup2(data->prevfd, STDIN_FILENO);
		dup2(data->pipe_fd[1], STDOUT_FILENO);
	}
	else if (i == data->pipes)
	{
		dup2(data->prevfd, STDIN_FILENO);
	}
	execve(data->cmd_path, data->cmd, envp);
}

void	ft_fd_close(t_data *data, int i)
{
	if (i != 0 && data->prevfd > 2)
	{
		close(data->prevfd);
		data->prevfd = -1;
	}
	if (i < data->pipes)
	{
		data->prevfd = data->pipe_fd[0];
		if (data->pipe_fd[1] > 0)
		{
			close(data->pipe_fd[1]);
			data->pipe_fd[1] = -1;
		}
	}
}

void	ft_minishell(t_data *data, char **envp)
{
	int	i;

	i = 0;
	if (data->pipes == 0)
	{
		data->cmd = ft_split(data->input, ' ');
		if (data->cmd[0])
		{
			data->path = ft_find_path(envp);
			data->path_dir = ft_split(data->path, ':');
			data->cmd_path = ft_cmd_path(data);
			if (data->cmd_path)
			{
				data->c_id = fork();
				if (data->c_id == 0)
				{
					execve(data->cmd_path, data->cmd, envp);
				}
			}
			else
			{
				ft_error();	
			}
		}
	}
	else if (data->pipes > 0)
	{
		while (i <= data->pipes)
		{
			data->cmd = ft_split(data->cmd_dir[i], ' ');
			data->path = ft_find_path(envp);
			data->path_dir = ft_split(data->path, ':');
			data->cmd_path = ft_cmd_path(data);
			if (data->cmd_path)
			{
				if (i < data->pipes)
					data->pipe_status = pipe(data->pipe_fd);
				if (data->pipe_status == -1)
					perror("pipe error");
				data->c_id = fork();
				if (data->c_id == 0)
					ft_child_minishell(data, envp, i);
			}
			else
				ft_error();
			ft_fd_close(data, i);
			i++;
		}
	}
	waitpid(data->c_id, NULL, 0);
}

int	ft_pipe_count(char *input)
{
	int	i;
	int	pipes;

	i = 0;
	pipes = 0;
	while (input[i])
	{
		if (input[i] == '|')
			pipes++;
		i++;
	}
	return (pipes);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	int		i;

	i = 0;
	if(argc > 0 && argv[0])
	{
		while (1)
		{
			data.input = readline("->");
			if (data.input)
			{
				add_history(data.input);
			}
			data.pipes = ft_pipe_count(data.input);
			if (data.pipes > 0)
				data.cmd_dir = ft_split(data.input, '|');
			ft_minishell(&data, envp);
			// if (data.pipes > 0)
			// {
			// 	data.cmd_dir = ft_split(data.input, '|');
			// 	while (i <= data.pipes + 1)
			// 	{
			// 		data.cmd = ft_split(data.cmd_dir[i], ' ');
			// 		data.path = ft_find_path(envp);
			// 		data.path_dir = ft_split(data.path, ':');
			// 		data.cmd_path = ft_cmd_path(&data);
			// 		if (data.cmd_path)
			// 		{
			// 			ft_minishell(&data, envp);	
			// 		}
			// 		else
			// 			ft_error();
			// 		i++;
			// 	}
			// }
			// else
			// {
			// 	data.cmd = ft_split(data.input, ' ');
			// 	data.path = ft_find_path(envp);
			// 	data.path_dir = ft_split(data.path, ':');
			// 	data.cmd_path = ft_cmd_path(&data);
			// 	if (data.cmd_path)
			// 		ft_minishell(&data, envp);
			// 	else
			// 		ft_error();	
			// }
		}
	}
	return (0);
}