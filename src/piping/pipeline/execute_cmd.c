/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 00:00:00 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/03 17:40:12 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

static void	cleanup_and_exit(char **argv, t_token **cmd, int status)
{
	g_exit_code = status;
	free_cmd(argv);
	free_tokens(cmd);
	exit(g_exit_code);
}

static char	**prepare_builtin_execution(char ***envp, t_token **cmd)
{
	char	**argv;

	argv = tokens_to_argv(cmd);
	if (!argv || !argv[0])
		cleanup_and_exit(argv, cmd, 0);
	if (is_builtin(argv[0]))
	{
		run_builtin(envp, cmd);
		cleanup_and_exit(argv, cmd, g_exit_code);
	}
	return (argv);
}

static void	check_direct_path(char **argv, char **envp)
{
	if (access(argv[0], F_OK) != 0)
	{
		error_perror(argv[0]);
		g_exit_code = 127;
	}
	else if (is_folder(argv[0]))
	{
		error_msg(argv[0], "Is a directory");
		g_exit_code = 126;
	}
	else if (access(argv[0], X_OK) != 0)
	{
		error_perror(argv[0]);
		g_exit_code = 126;
	}
	else
	{
		execve(argv[0], argv, envp);
		error_perror(argv[0]);
		g_exit_code = exit_code_from_errno();
	}
}

static void	search_path_and_exec(char **argv, char **envp)
{
	char	*path;

	path = get_path(envp, argv);
	if (path)
	{
		execve(path, argv, envp);
		error_perror(argv[0]);
		free(path);
		g_exit_code = exit_code_from_errno();
	}
	else
	{
		error_msg(argv[0], "command not found");
		g_exit_code = 127;
	}
}

void	execute_cmd(char **envp, t_token **cmd)
{
	char	**argv;

	argv = prepare_builtin_execution(&envp, cmd);
	if (ft_strchr(argv[0], '/'))
		check_direct_path(argv, envp);
	else
		search_path_and_exec(argv, envp);
	cleanup_and_exit(argv, cmd, g_exit_code);
}
