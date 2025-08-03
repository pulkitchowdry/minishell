/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:32:25 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/02 21:43:54 by chikoh           ###   ########.fr       */
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

enum e_token_type
{
	DOUBLE_QUOTE,
	DOUBLE_QUOTE_STRING,
	SINGLE_QUOTE,
	SINGLE_QUOTE_STRING,
	COMMAND,
	HERE_DOC,
	REDIRECT_INPUT,
	REDIRECT_OUPUT,
	REDIRECT_APPEND,
	PIPE,
	LOGICAL_AND,
	LOGICAL_OR,
	VARIABLE,
	WILDCARD,
	SPACES,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	ARGUMENTS,
	STRING
};

typedef struct s_token
{
	enum e_token_type	type;
	char		*string;
}	t_token;

typedef struct s_data
{
	char	*input;
	char	**cmd_dir;
	char	**cmd;
	char	*path;
	char	**path_dir;
	char	*p_temp;
	char	*cmd_path;
	pid_t	c_id;
	int		pipes;
	int		pipe_status;
	int		pipe_fd[2];
	int		prevfd;
}	t_data;

char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *str);
t_list	*create_tokens(char *string);

#endif
