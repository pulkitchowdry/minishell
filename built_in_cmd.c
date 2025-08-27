/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchowdry <pchowdry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:11:34 by pchowdry          #+#    #+#             */
/*   Updated: 2025/08/27 13:41:53 by pchowdry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "libft/get_next_line.h"

void	ft_echo(t_list *command, t_list *redirection, t_variable_context *context)
{
		//Displays the text provided in the input or variable. We can get inputs like below
		//1. echo "test" testing "another" - This output like test testing another - Ignore quotes
		//2. echo "value of 'a':$a"-$a-check - This outputs value of 'a':5-5-check if a=5
		//3. echo "value of \"a\"" - This outputs as value of "a"
		//4. echo -n test - The -n flag does not print a new line after the output is printed
		t_list	*temp_cmd;
		t_env	temp_envp;

		temp_cmd = command;
		ft_bzero(&temp_envp, sizeof(temp_envp));
		if (temp_cmd->next)
		{
			temp_cmd = temp_cmd->next;
			if (ft_strncmp(temp_cmd->content, "-n", ft_strlen(temp_cmd->content)) == 0)
			{
				temp_envp.no_new_line_flag = 1;
				if (temp_cmd->next)
					temp_cmd = temp_cmd->next;
				else
					write(1, "", 0);
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

void	ft_export(t_list *command, t_list *redirection, t_variable_context *context)
{
		//Export command - We can have two methods, 1. export a or 2. export a=1
		//When we receive just export a, then we need to check local var to check if a value exists.
		//If a value exists then we need to add var in export along with the value, if not then only add the key "a". We do not add to envp
		//When we receive export a=1, then we need to add the key and value to export and envp
		//Another case if export a -> a=1 then we need to add the value 1 to the variable in export and envp along with the key which got added first
		t_list	*temp_cmd;
		t_env	temp_envp;
		int		i;
		int		j;
		
		temp_cmd = command;
		ft_bzero(&temp_envp, sizeof(temp_envp));
		if (temp_cmd->next)
		{
			//When the variable is given then we go here
			temp_cmd = temp_cmd->next;
			while (temp_cmd)
			{
				temp_envp.new_env = ft_split(temp_cmd->content, '=');
				j = 0;
				while (temp_envp.new_env[j] != NULL)
				{
					if (j == 0)
						temp_envp.new_env_key = temp_envp.new_env[j];
					else if (j == 1)
						temp_envp.new_env_value = temp_envp.new_env[j];
					else
					{
						temp_envp.new_env_value = ft_strjoin(temp_envp.new_env_value, "=");
						temp_envp.new_env_value = ft_strjoin(temp_envp.new_env_value, temp_envp.new_env[j]);
					}
					j++;
				}
				//Variable is given but there is no value provided then we go here next
				if (j == 1)
				{
					i = 0;
					while (context->local_variables && context->local_variables[i] != NULL)
					{
						temp_envp.new_env = ft_split(context->local_variables[i], '=');
						if (ft_strncmp(temp_envp.new_env_key,
							temp_envp.new_env[0], ft_strlen(temp_envp.new_env[0]) + 1) == 0)
						{
							j = 0;
							while (temp_envp.new_env[j] != NULL)
							{
								printf("J: %d - %s\n", j, temp_envp.new_env[j]);
								if (j == 1)
									temp_envp.new_env_value = temp_envp.new_env[j];
								else if (j > 1)
								{
									temp_envp.new_env_value = ft_strjoin(temp_envp.new_env_value, "=");
									temp_envp.new_env_value = ft_strjoin(temp_envp.new_env_value, temp_envp.new_env[j]);
								}
								j++;
							}
						}	
						i++;
					}
				}
				// context->local_variables = ft_add_to_local(&temp_envp, context);
				append_local_variables(command, redirection, context);
				ft_update_envp(&temp_envp, command, context);
				temp_cmd = temp_cmd->next;
			}
		}
		else if (!temp_cmd->next)
		{
			//Need to display environment variables with declare -x infront
			//And values within quotes
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
	
	temp_cmd = command;
	ft_bzero(&temp_envp, sizeof(temp_envp));
	i = 0;
	if (temp_cmd->next)
	{
		temp_cmd = temp_cmd->next;
		while (temp_cmd)
		{
			temp_envp.char_unset = ft_strjoin(temp_cmd->content, "=");
			while (context->environment_variables[i])
			{
				if (ft_strncmp(context->environment_variables[i], temp_envp.char_unset, ft_strlen(temp_envp.char_unset)) == 0)
					context->environment_variables[i] = NULL;
				i++;
			}
			i = 0;
			temp_envp.char_unset = ft_strdup(temp_cmd->content);
			temp_envp.char_unset = ft_strjoin("declare -x ", temp_envp.char_unset);
			temp_envp.char_unset = ft_strjoin(temp_envp.char_unset, "=");
			while (context->dup_environment_variables[i])
			{
				if (ft_strncmp(context->dup_environment_variables[i], temp_envp.char_unset, ft_strlen(temp_envp.char_unset)) == 0)
					context->dup_environment_variables = ft_remove_from_myenvp(context, &temp_envp);
				i++;
			}
			temp_cmd = temp_cmd->next;
		}
	}
}
