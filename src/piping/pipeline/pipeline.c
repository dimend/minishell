/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:39:33 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/26 16:00:00 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_pipe_if_needed(t_pipeline *p, int *fd, int i)
{
	if (i < p->nbr_segments - 1)
	{
		if (pipe(fd) == -1)
		{
			error_perror("pipe");
			return (0);
		}
	}
	return (1);
}

static int	pipeline_step(t_pipeline *pipeline, int *in_fd, int *fd, int i)
{
	t_pipe_context	pipe_ctx;

	if (pipeline->cmds[i].in_fd != STDIN_FILENO && *in_fd != STDIN_FILENO)
	{
		close(*in_fd);
		*in_fd = STDIN_FILENO;
	}
	if (!create_pipe_if_needed(pipeline, fd, i))
		return (0);
	pipe_ctx.in_fd = *in_fd;
	pipe_ctx.fd = fd;
	pipe_ctx.last = (i == pipeline->nbr_segments - 1);
	spawn_child(pipeline, pipe_ctx, i);
	parent_cleanup(in_fd, fd, i, pipeline->nbr_segments);
	return (1);
}

void	pipeline_loop(t_pipeline *pipeline)
{
	int	in_fd;
	int	fd[2];
	int	i;

	in_fd = STDIN_FILENO;
	i = 0;
	while (i < pipeline->nbr_segments)
	{
		if (!pipeline_step(pipeline, &in_fd, fd, i))
			break ;
		i++;
	}
	if (in_fd != STDIN_FILENO)
		close(in_fd);
}
