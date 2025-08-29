/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:11:34 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/29 15:05:24 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

extern int	g_ret_code;

void	ft_echo(t_list *command, t_list *redirection, t_variable_context *context)
{
		t_list	*temp_cmd;
		t_env	temp_envp;

		temp_cmd = command;
		ft_bzero(&temp_envp, sizeof(temp_envp));
		if (temp_cmd->next)
		{
			temp_cmd = temp_cmd->next;
			if (ft_strlen(temp_cmd->content) > 0 && ft_strncmp(temp_cmd->content, "-n", ft_strlen(temp_cmd->content)) == 0)
			{
				temp_envp.no_new_line_flag = 1;
				write(1, "", 0);
				temp_cmd = temp_cmd->next;
			}
			while (temp_cmd)
			{
				write(1, temp_cmd->content, ft_strlen(temp_cmd->content));
				if (temp_cmd->next)
					write(1, " ", 1);
				temp_cmd = temp_cmd->next;
			}
		}
		if (temp_envp.no_new_line_flag != 1)
			write(1, "\n", 1);
}

void	ft_cd(t_list *command, t_list *redirection, t_variable_context *context)
{
	t_list	*temp_cmd;
	t_env	temp_envp;

	temp_cmd = command;
	ft_bzero(&temp_envp, sizeof(temp_envp));
	if (temp_cmd->next && chdir(temp_cmd->next->content) == 0)
	{
		temp_envp.pwd = getcwd(NULL, 0);
		temp_envp.oldpwd = ft_extract_envp(context->environment_variables, "PWD=");
		ft_update_envp(&temp_envp, command, context);
	}
	else if (!temp_cmd->next)
	{
		if (chdir(getenv("HOME")) == 0)
		{
			temp_envp.pwd = getcwd(NULL, 0);
			temp_envp.oldpwd = ft_extract_envp(context->environment_variables, "PWD=");
			ft_update_envp(&temp_envp, command, context);
		}
	}
	else
		perror("cd");
}
void	ft_pwd(t_list *command, t_list *redirection, t_variable_context *context)
{
	char	*string;

	string = ft_calloc(sizeof(char), ft_strlen(getcwd(NULL, 0)));
	if (!string)
		perror("malloc failed");
	string = ft_strdup(getcwd(NULL, 0));
	if (string)
		printf("%s\n", string);
	else
		perror("pwd");
}

void	ft_export(t_list *command, t_list *redirection, t_variable_context *context)
{
		t_list	*temp_cmd;
		t_env	temp_envp;
		int		i;
		int		j;
		
		temp_cmd = command;
		ft_bzero(&temp_envp, sizeof(temp_envp));
		if (temp_cmd->next)
		{
			temp_cmd = temp_cmd->next;
			while (temp_cmd)
			{
				temp_envp.new_env_key = ft_get_key(temp_cmd->content);
				temp_envp.new_env_value = ft_get_value(temp_cmd->content);
				if (!temp_envp.new_env_value)
				{
					i = 0;
					while (context->local_variables && context->local_variables[i] != NULL)
					{
						temp_envp.local_key = ft_get_key(context->local_variables[i]);
						temp_envp.local_value = ft_get_value(context->local_variables[i]);
						if (ft_strncmp(temp_envp.new_env_key,
							temp_envp.local_key, ft_strlen(temp_envp.local_key) + 1) == 0)
						{
							if (temp_envp.local_value)
								temp_envp.new_env_value = temp_envp.local_value;
						}
						i++;
					}
				}
				append_local_variables_2(command, redirection, context);
				ft_update_envp(&temp_envp, command, context);
				temp_cmd = temp_cmd->next;
			}
		}
		else if (!temp_cmd->next)
		{
			i = 0;
			while (context->dup_environment_variables[i])
			{
				write(1, context->dup_environment_variables[i], ft_strlen(context->dup_environment_variables[i]));
				write(1, "\n", 1);
				i++;
			}
			return ;
		}
}

void	ft_unset(t_list *command, t_list *redirection, t_variable_context *context)
{
	t_list	*temp_cmd;
	t_env	temp_envp;
	int		i;
	char	*extract;
	
	temp_cmd = command;
	ft_bzero(&temp_envp, sizeof(temp_envp));
	i = 0;
	if (temp_cmd->next)
	{
		temp_cmd = temp_cmd->next;
		while (temp_cmd)
		{
			// temp_envp.char_unset = ft_strjoin(temp_cmd->content, "=");
			temp_envp.char_unset = ft_strdup(temp_cmd->content);
			i = 0;
			while (context->environment_variables && context->environment_variables[i])
			{
				extract = ft_get_key(context->environment_variables[i]);
				if (ft_strncmp(extract, temp_envp.char_unset, ft_strlen(extract) + 1) == 0)
					context->environment_variables = ft_remove_from_dup_envp(context, &temp_envp);
				i++;
			}
			i = 0;
			while (context->local_variables && context->local_variables[i])
			{
				extract = ft_get_key(context->local_variables[i]);
				if (ft_strncmp(extract, temp_envp.char_unset, ft_strlen(extract) + 1) == 0)
					context->local_variables = ft_remove_from_local(context, &temp_envp);
				i++;
			}
			i = 0;
			temp_envp.char_unset = ft_strjoin("declare -x ", temp_envp.char_unset);
			while (context->dup_environment_variables && context->dup_environment_variables[i])
			{
				extract = ft_get_key(context->dup_environment_variables[i]);
				if (ft_strncmp(extract, temp_envp.char_unset, ft_strlen(extract) + 1) == 0)
					context->dup_environment_variables = ft_remove_from_myenvp(context, &temp_envp);
				i++;
			}
			free(extract);
			temp_cmd = temp_cmd->next;
		}
	}
}

void	ft_env(t_list *command, t_list *redirection, t_variable_context *context)
{
	int	i;

	i = 0;
	while (context->environment_variables[i])
	{
		write(1, context->environment_variables[i], ft_strlen(context->environment_variables[i]));
		write(1, "\n", 1);
		i++;
	}
}

void	ft_exit(t_ast_node *root,
		t_list *token, t_ast_node *node,
		t_variable_context *context)
{
	rl_clear_history();
	free_command(&root);
	ft_lstclear(&token, free_token);
	free_string_array(context->environment_variables);
	free_string_array(context->dup_environment_variables);
	free_string_array(context->local_variables);
	printf("exit\n");
	exit(g_ret_code);
}
