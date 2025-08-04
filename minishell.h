/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:32:25 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/04 12:13:52 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>

typedef struct s_data
{
	char	*input; //User input
	char	**cmd_dir; //Store all commands in case of pipe
	char	**cmd; //Store specific command and its flags
	char	*path; //Store path /usr/bin
	char	**path_dir; //Complete path directory from env
	char	*p_temp; //Temp variable to get cmd path
	char	*cmd_path; //Store command path
	pid_t	c_id; //Store child process id
	int		pipes; //Number of pipes
	int		pipe_status; //Pipe status
	int		pipe_fd[2]; //Pipe file descriptors
	int		prevfd; //Preview pipe file descriptor
	char	**myenvp; //For export to duplicate envp
}	t_data;

typedef struct s_env
{
	char	*oldpwd; //For cd
	char	*pwd; //For cd
	char	**new_env; //For export to store both key and value
	char	*new_env_key; //For export
	char	*new_env_value; //For export
	// char	**myenvp; //For export to duplicate envp
}	t_env;

char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	ft_bzero(void *str, size_t n);
char	*ft_strdup(const char *s);
int		ft_pipe_count(char *input);
void	ft_minishell(t_data *data, char **envp);
void	ft_fd_close(t_data *data, int i);
void	ft_child_minishell(t_data *data, char **envp, int i);
char	*ft_cmd_path(t_data *data);
char	*ft_find_path(char **envp);
void	ft_error(void);
void	print_signal(int signal);
int		is_builtin(char *str);
void	ft_builtin_exec(t_data *data, char **envp);
char	*ft_extract_envp(char **envp, char *str);
void	ft_update_envp(t_env *temp_envp, t_data *data, char **envp);
char	**ft_dup_envp(char **envp);
char	**ft_copy_envp(char **envp, char *new);
size_t	ft_env_length(char **envp);

#endif