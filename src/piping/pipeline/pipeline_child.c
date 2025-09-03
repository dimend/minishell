/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 16:00:00 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/26 16:00:00 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_process(char **envp, t_command *cmd, t_pipe_context pipe_ctx)
{
	handle_input_redirection(cmd->in_fd, &pipe_ctx.in_fd);
	handle_output_redirection(cmd->out_fd, pipe_ctx.fd, pipe_ctx.last);
	if (!pipe_ctx.last)
	{
		close(pipe_ctx.fd[0]);
		close(pipe_ctx.fd[1]);
	}
	execute_cmd(envp, cmd->tokens);
	exit(0);
}

void	spawn_child(t_pipeline *p, t_pipe_context ctx, int i)
{
	if (p->cmds[i].tokens)
	{
		p->pids[i] = fork();
		if (p->pids[i] == 0)
			child_process(p->envp, &p->cmds[i], ctx);
	}
	else
		p->pids[i] = -1;
}
