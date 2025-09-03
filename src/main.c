/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:41:29 by dimendon          #+#    #+#             */
/*   Updated: 2025/09/03 19:17:12 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

int			g_exit_code = 0;

void	sigint_handler(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_code = 130;
}

static void	run_shell(char ***env)
{
	char	*line;
	char	*tmp;

	while (1)
	{
		if (isatty(fileno(stdin)))
			line = readline("minishell$ ");
		else
			line = readline("");
		if (!line)
			break ;
		tmp = ft_strdup(line);
		free(line);
		line = tmp;
		if (*line != '\0')
		{
			add_history(line);
			process_command(env, line);
		}
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	**env;

	(void)argc;
	(void)argv;
	env = copy_envp(envp);
	if (!isatty(fileno(stdin)))
		rl_outstream = fopen("/dev/null", "w");
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	run_shell(&env);
	free_cmd(env);
	return (g_exit_code);
}
