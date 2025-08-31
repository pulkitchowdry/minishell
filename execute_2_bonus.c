/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_2_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:00:51 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/31 15:21:13 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

extern int	g_ret_code;

int	open_temp_files(t_ast_node *root, t_list *tokens, t_list *list,
					t_variable_context *context)
{
	char	*redirect_type;
	int		ret_code;

	ret_code = 0;
	while (list != 0 && ret_code == 0)
	{
		redirect_type = (char *)list->content;
		if (ft_strncmp("<<", redirect_type, 3) == 0)
		{
			ret_code = fork_heredoc(root, tokens, list, context);
			if (ret_code == 2)
				return (ret_code);
		}
		list = list->next->next;
	}
	return (ret_code);
}

void	append_backslash_to_string_array(char **string_array)
{
	int		i;
	char	*string;

	i = 0;
	while (string_array[i])
	{
		string = string_array[i];
		string_array[i] = ft_strjoin(string, "/");
		free(string);
		i++;
	}
}

char	**extract_path_variable(char **environment_variable)
{
	char	**result;
	char	*key;
	char	*value;
	int		i;

	i = 0;
	key = 0;
	value = 0;
	while (environment_variable[i] && value == 0)
	{
		key = get_variable_key(environment_variable[i]);
		if (ft_strncmp("PATH", key, ft_strlen(key) + 1) == 0)
			value = get_variable_value(environment_variable[i]);
		free(key);
		i++;
	}
	if (value == 0)
		return (0);
	else
		result = ft_split(value, ':');
	append_backslash_to_string_array(result);
	free(value);
	return (result);
}

int	print_permission_denied_child(t_ast_node *root, t_list *tokens,
	char *string, t_variable_context *context)
{
	perror(string);
	unlink_files(root);
	free_command(&root);
	ft_lstclear(&tokens, free_token);
	free_string_array(context->environment_variables);
	free_string_array(context->local_variables);
	free_string_array(context->dup_environment_variables);
	close(0);
	close(1);
	close(2);
	exit(1);
}

void	compare_redirect_input_child(t_ast_node *root, t_list *tokens,
	t_list *redirection, t_variable_context *context)
{
	int	fd;

	if (ft_strncmp("<", (char *)redirection->content, 2) == 0)
	{
		fd = open((char *)redirection->next->content, O_RDONLY);
		if (fd == -1)
			print_permission_denied_child(root, tokens,
				(char *)redirection->next->content, context);
		else
		{
			dup2(fd, 0);
			close(fd);
		}
	}
}
