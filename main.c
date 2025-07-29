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

int	main(int argc, char **argv, char **envp)
{
	char	*string_cmd;

	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, print_signal);
	string_cmd = readline("jdon my soul >");
	while (string_cmd)
	{
		add_history(string_cmd);
		string_cmd = readline("jdon my soul >");
	}
	return (0);
}
