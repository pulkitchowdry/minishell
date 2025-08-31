/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_4_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:03:51 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/31 15:21:19 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

extern int	g_ret_code;

void	print_no_file_or_directory(char *command,
		t_list *tokens, t_ast_node *root,
		t_variable_context *context)
{
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	free(command);
	unlink_files(root);
	free_command(&root);
	ft_lstclear(&tokens, free_token);
	free_string_array(context->environment_variables);
	free_string_array(context->local_variables);
	free_string_array(context->dup_environment_variables);
	close(0);
	close(1);
	close(2);
	exit(127);
}

void	substitute_node_with_list(t_list **result,
		t_list **prev, t_list **command)
{
	t_list	*match;

	match = find_match_string((char *)(*command)->content);
	if (match == 0)
	{
		*prev = *command;
		*command = (*command)->next;
		return ;
	}
	if (*prev == 0)
	{
		*result = match;
		*prev = ft_lstlast(match);
	}
	else
	{
		(*prev)->next = match;
		*prev = ft_lstlast(*prev);
	}
	(*prev)->next = (*command)->next;
	ft_lstdelone(*command, free);
	*command = (*prev)->next;
}

t_list	*expand_command_wildcard_and_quotes(t_list *cmd,
		t_variable_context *context)
{
	t_list	*result;
	t_list	*prev;

	prev = 0;
	result = cmd;
	while (cmd)
	{
		if (((char *)cmd->content)[0] == '"' && ((char *)cmd->content)[
			ft_strlen((char *)cmd->content) - 1] == '"')
			substitute_node_for_double_quote(&prev, &cmd, context);
		else if (((char *)cmd->content)[0] == '\'' && ((char *)cmd->content)[
			ft_strlen((char *)cmd->content) - 1] == '\'')
			substitute_node_for_single_quote(&prev, &cmd);
		else if (((char *)cmd->content)[0] == '$')
			substitute_node_for_variable(&result, &prev, &cmd, context);
		else if (is_wildcard_present((char *)cmd->content))
			substitute_node_with_list(&result, &prev, &cmd);
		else
		{
			prev = cmd;
			cmd = cmd->next;
		}
	}
	return (result);
}

void	search_and_exec(t_ast_node *root,
		t_list *tokens, t_ast_node *node,
		t_variable_context *context)
{
	char	**path_values;
	char	*path;
	char	*command_name;
	int		search;

	configure_redirection_child(root, tokens, node->redirection, context);
	if (access((char *)node->command->content, X_OK) == 0)
		execute_with_execve(node->command, context->environment_variables);
	command_name = ft_strdup((char *)node->command->content);
	path_values = extract_path_variable(context->environment_variables);
	if (path_values == 0)
		print_no_file_or_directory(command_name, tokens, root, context);
	search = 0;
	while (path_values[search])
	{
		path = ft_strjoin(path_values[search++], command_name);
		free(node->command->content);
		node->command->content = path;
		if (access(path, X_OK) == 0)
			execute_with_execve(node->command, context->environment_variables);
	}
	free_string_array(path_values);
	print_no_file_or_directory(command_name, tokens, root, context);
}

int	fork_and_wait(t_ast_node *root,
		t_list *tokens, t_ast_node *node,
		t_variable_context *context)
{
	int	pid;
	int	ret_code;

	ret_code = 0;
	pid = fork();
	if (pid == 0)
		search_and_exec(root, tokens, node, context);
	else
	{
		signal(SIGINT, print_new_line);
		waitpid(pid, &ret_code, 0);
		signal(SIGINT, print_signal);
	}
	if (WIFEXITED(ret_code))
		return (WEXITSTATUS(ret_code));
	if (WIFSIGNALED(ret_code))
		return (128 + WTERMSIG(ret_code));
	return (ret_code);
}
