/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 00:00:00 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/02 16:55:50 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipe(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}

void	parent_cleanup(int *in_fd, int *fd, int i, int num)
{
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	if (i < num - 1)
	{
		close(fd[1]);
		*in_fd = fd[0];
	}
}

static int	get_exit_code(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			return (130);
		if (WTERMSIG(status) == SIGQUIT)
			return (131);
		return (1);
	}
	return (1);
}

void	wait_for_all(pid_t *pids, int count)
{
	int	i;
	int	status;
	int	code;
	int	last_code;

	last_code = 0;
	i = 0;
	while (i < count)
	{
		if (pids[i] > 0 && waitpid(pids[i], &status, 0) != -1)
		{
			code = get_exit_code(status);
			if (i == count - 1)
				last_code = code;
		}
		else if (pids[i] == -1 && i == count - 1)
			last_code = 1;
		else if (pids[i] > 0)
			error_perror("waitpid");
		i++;
	}
	g_exit_code = last_code;
}

void	cleanup_commands(t_pipeline *pipeline, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (pipeline->cmds[i].in_fd != STDIN_FILENO)
			close(pipeline->cmds[i].in_fd);
		if (pipeline->cmds[i].out_fd != STDOUT_FILENO)
			close(pipeline->cmds[i].out_fd);
		free_tokens(pipeline->cmds[i].tokens);
		i++;
	}
	free(pipeline->cmds);
}
