/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:11:34 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/29 22:39:15 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

extern int	g_ret_code;

void	ft_echo(t_list *command)
{
	t_list	*t_cmd;
	t_env	temp_envp;

	t_cmd = command;
	ft_bzero(&temp_envp, sizeof(temp_envp));
	if (t_cmd->next)
	{
		t_cmd = t_cmd->next;
		if (ft_strlen(t_cmd->content) > 0
			&& ft_strncmp(t_cmd->content, "-n", ft_strlen(t_cmd->content)) == 0)
		{
			temp_envp.no_new_line_flag = 1;
			write(1, "", 0);
			t_cmd = t_cmd->next;
		}
		while (t_cmd)
		{
			write(1, t_cmd->content, ft_strlen(t_cmd->content));
			if (t_cmd->next)
				write(1, " ", 1);
			t_cmd = t_cmd->next;
		}
	}
	if (temp_envp.no_new_line_flag != 1)
		write(1, "\n", 1);
}

void	ft_cd(t_list *command, t_variable_context *context)
{
	t_list	*temp_cmd;
	t_env	temp_envp;

	temp_cmd = command;
	ft_bzero(&temp_envp, sizeof(temp_envp));
	if (temp_cmd->next && chdir(temp_cmd->next->content) == 0)
	{
		temp_envp.pwd = getcwd(NULL, 0);
		temp_envp.oldpwd = ft_extract_envp(context->environment_variables,
				"PWD=");
		ft_update_envp(&temp_envp, command, context);
	}
	else if (!temp_cmd->next)
	{
		if (chdir(getenv("HOME")) == 0)
		{
			temp_envp.pwd = getcwd(NULL, 0);
			temp_envp.oldpwd = ft_extract_envp(context->environment_variables,
					"PWD=");
			ft_update_envp(&temp_envp, command, context);
		}
	}
	free(temp_envp.pwd);
	free(temp_envp.oldpwd);
}

void	ft_pwd(void)
{
	char	*string;

	string = getcwd(NULL, 0);
	if (string)
		printf("%s\n", string);
	else
		ft_putstr_fd("pwd error", 2);
	free(string);
}

void	ft_export_3(t_env *temp_envp, t_variable_context *context)
{
	int	i;

	i = 0;
	while (context->local_variables && context->local_variables[i] != NULL)
	{
		temp_envp->local_key = ft_get_key(context->local_variables[i]);
		temp_envp->local_value = ft_get_value(context->local_variables[i]);
		if (ft_strncmp(temp_envp->new_env_key, temp_envp->local_key,
				ft_strlen(temp_envp->local_key) + 1) == 0)
		{
			if (temp_envp->local_value)
				temp_envp->new_env_value = ft_strdup(temp_envp->local_value);
		}
		free(temp_envp->local_key);
		free(temp_envp->local_value);
		i++;
	}
}

void	ft_export_2(t_list *command, t_list *redirection,
						t_variable_context *context)
{
	t_list	*temp_cmd;
	t_env	temp_envp;

	temp_cmd = command;
	ft_bzero(&temp_envp, sizeof(temp_envp));
	temp_cmd = temp_cmd->next;
	while (temp_cmd)
	{
		temp_envp.new_env_key = ft_get_key(temp_cmd->content);
		temp_envp.new_env_value = ft_get_value(temp_cmd->content);
		if (!temp_envp.new_env_value)
			ft_export_3(&temp_envp, context);
		append_local_variables_2(command, redirection, context);
		ft_update_envp(&temp_envp, command, context);
		free(temp_envp.new_env_key);
		free(temp_envp.new_env_value);
		temp_cmd = temp_cmd->next;
	}
}

void	ft_export(t_list *command, t_list *redirection,
					t_variable_context *context)
{
	int	i;

	i = 0;
	if (command->next)
		ft_export_2(command, redirection, context);
	else if (!command->next)
	{
		while (context->dup_environment_variables[i])
		{
			write(1, context->dup_environment_variables[i],
				ft_strlen(context->dup_environment_variables[i]));
			write(1, "\n", 1);
			i++;
		}
	}
}

void	ft_unset_dup(t_env *temp_envp, t_variable_context *context)
{
	int		i;
	char	*extract;
	char	*temp;

	i = 0;
	temp = ft_strjoin("declare -x ", temp_envp->char_unset);
	free(temp_envp->char_unset);
	temp_envp->char_unset = temp;
	while (context->dup_environment_variables
		&& context->dup_environment_variables[i])
	{
		extract = ft_get_key(context->dup_environment_variables[i]);
		if (ft_strncmp(extract, temp_envp->char_unset,
				ft_strlen(extract) + 1) == 0)
			context->dup_environment_variables
				= ft_remove_from_myenvp(context, temp_envp);
		free(extract);
		i++;
	}
}

void	ft_unset_env(t_env *temp_envp, t_variable_context *context)
{
	int		i;
	char	*extract;

	i = 0;
	while (context->environment_variables && context->environment_variables[i])
	{
		extract = ft_get_key(context->environment_variables[i]);
		if (ft_strncmp(extract, temp_envp->char_unset,
				ft_strlen(extract) + 1) == 0)
			context->environment_variables
				= ft_remove_from_dup_envp(context, temp_envp);
		free(extract);
		i++;
	}
	i = 0;
	while (context->local_variables && context->local_variables[i])
	{
		extract = ft_get_key(context->local_variables[i]);
		if (ft_strncmp(extract, temp_envp->char_unset,
				ft_strlen(extract) + 1) == 0)
			context->local_variables = ft_remove_from_local(context, temp_envp);
		free(extract);
		i++;
	}
}

void	ft_unset(t_list *command,
					t_variable_context *context)
{
	t_list	*temp_cmd;
	t_env	temp_envp;

	temp_cmd = command;
	ft_bzero(&temp_envp, sizeof(temp_envp));
	if (temp_cmd->next)
	{
		temp_cmd = temp_cmd->next;
		while (temp_cmd)
		{
			temp_envp.char_unset = ft_strdup(temp_cmd->content);
			ft_unset_env(&temp_envp, context);
			ft_unset_dup(&temp_envp, context);
			free(temp_envp.char_unset);
			temp_cmd = temp_cmd->next;
		}
	}
}

void	ft_env(t_variable_context *context)
{
	int	i;

	i = 0;
	while (context->environment_variables[i])
	{
		write(1, context->environment_variables[i],
			ft_strlen(context->environment_variables[i]));
		write(1, "\n", 1);
		i++;
	}
}

void	ft_exit(t_ast_node *root,
		t_list *token, t_ast_node *node,
		t_variable_context *context)
{
	if (node->redirection)
	{
		close(1023);
		close(1022);
	}
	rl_clear_history();
	free_command(&root);
	unlink_files(root);
	ft_lstclear(&token, free_token);
	free_string_array(context->environment_variables);
	free_string_array(context->dup_environment_variables);
	free_string_array(context->local_variables);
	ft_putstr_fd("exit\n", 2);
	close(0);
	close(1);
	close(2);
	exit(g_ret_code);
}
