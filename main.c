/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:08:31 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/29 16:07:59 by chikoh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>

#include "libft/libft.h"
#include "minishell.h"

int		g_ret_code = 0;

void	print_signal(int signal)
{
	(void)signal;
	rl_on_new_line();
	printf("\n");
	rl_replace_line("", 0);
	rl_redisplay();
}

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

void	print_string_list(t_list *list)
{
	while (list != 0)
	{
		printf("%s ", (char *)list->content);
		list = list->next;
	}
}

void	print_ast(t_ast_node *node)
{
	if (node == 0)
		return ;
	printf("(");
	print_ast(node->left);
	if (node->node != 0)
		printf("node: %s", node->node->string);
	if (node->command != 0)
	{
		printf("command: ");
		print_string_list(node->command);
	}
	if (node->redirection != 0)
	{
		printf("redirection: ");
		print_string_list(node->redirection);
	}
	if (node->assignment != 0)
	{
		printf("assignment: ");
		print_string_list(node->assignment);
	}
	print_ast(node->right);
	printf(")");
}

int	main(int argc, char **argv, char **envp)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, print_signal);
	t_data	data;
	t_list	*list_start;
	t_list	*list;
	t_variable_context	context;
	t_state_context		state_context;
	
	g_ret_code = 0;
	state_context.context = &context;
	state_context.current = 0;
	context.environment_variables = ft_dup_str_array(envp);
	context.local_variables = 0;
	context.dup_environment_variables = ft_dup_envp(envp);
	if(argc > 0 && argv[0])
	{
		while (1)
		{
			data.input = readline("->");
			if (data.input)
			{
				add_history(data.input);
				list = create_tokens(data.input);
				list_start = list;
				t_ast_node *root = parse_list(&list, &state_context);
				if (root != 0 && is_safe_to_execute(root) && list == 0)
					printf("The command is valid\n");
				else
					printf("The command cannot execute\n");
				g_ret_code = execute_heredoc(root, list_start, root, 0);
				if (g_ret_code == 0)
				{
					printf("heredoc is valid\n");
					g_ret_code = execute_command_ast(root, list_start, root, state_context.context);
				}
				else
					printf("heredoc is invalid\n");
				print_ast(root);
				while (list)
				{
					t_token *tok = (t_token *)list->content;
					printf("%s\n", tok->string);
					list = list->next;
				}
				unlink_files(root);
				free_command(&root);
				ft_lstclear(&list_start, free_token);
				free(data.input);
			}
			else
			{
				free(data.input);
				break ;
			}
		}
		rl_clear_history();
		free_string_array(context.environment_variables);
		free_string_array(context.dup_environment_variables);
		free_string_array(context.local_variables);
		close(0);
		close(1);
		close(2);
	}
	return (0);
}
