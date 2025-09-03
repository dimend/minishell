/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_processor.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 14:15:10 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/30 00:00:00 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"
#include <fcntl.h>

static void	execute_command_with_path(t_token **cmd, char ***envp)
{
	char	*path;

	path = get_path(*envp, prepare_argv_from_tokens(cmd));
	if (path)
	{
		g_exit_code = execute_command(path, cmd, *envp);
		free(path);
	}
	else
	{
		error_msg(cmd[0]->str, "command not found");
		g_exit_code = 127;
	}
}

static void	run_external_command(t_token **cmd, char ***envp)
{
	if (ft_strchr(cmd[0]->str, '/'))
	{
		if (access(cmd[0]->str, F_OK) != 0)
		{
			error_perror(cmd[0]->str);
			g_exit_code = 127;
		}
		else if (is_folder(cmd[0]->str))
		{
			error_msg(cmd[0]->str, "Is a directory");
			g_exit_code = 126;
		}
		else if (access(cmd[0]->str, X_OK) != 0)
		{
			error_perror(cmd[0]->str);
			g_exit_code = 126;
		}
		else
			g_exit_code = execute_command(cmd[0]->str, cmd, *envp);
	}
	else
		execute_command_with_path(cmd, envp);
}

static void	run_single(char ***envp, char *segment)
{
	t_token	**cmd;
	int		in_fd;
	int		out_fd;
	int		save_in;
	int		save_out;

	in_fd = STDIN_FILENO;
	out_fd = STDOUT_FILENO;
	save_in = 0;
	save_out = 0;
	cmd = prepare_command(segment, &in_fd, &out_fd, envp);
	if (cmd == (t_token **)-1)
		return ;
	if (!cmd || !cmd[0] || !cmd[0]->str)
		return ;
	setup_redirections(in_fd, out_fd, &save_in, &save_out);
	if (is_builtin(cmd[0]->str))
		run_builtin(envp, cmd);
	else
		run_external_command(cmd, envp);
	restore_redirections(save_in, save_out);
	free_tokens(cmd);
}

void	process_command(char ***envp, char *line)
{
	char	**segments;
	int		count;

	segments = split_pipes(line);
	if (!segments)
		return ;
	count = count_strings(segments);
	if (count > 1)
		execute_pipeline(*envp, segments);
	else
		run_single(envp, segments[0]);
	free_cmd(segments);
}
