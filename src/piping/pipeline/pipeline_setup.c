/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_setup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:39:38 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/26 16:00:00 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

static int	init_pipeline(t_pipeline *pipeline, char **envp, char **segments)
{
	pipeline->envp = envp;
	pipeline->segments = segments;
	pipeline->nbr_segments = count_strings(segments);
	pipeline->cmds = ft_calloc(pipeline->nbr_segments, sizeof(t_command));
	if (!pipeline->cmds)
	{
		g_exit_code = 1;
		return (0);
	}
	return (1);
}

static int	setup_commands(t_pipeline *p, char **envp)
{
	int	i;
	int	res;

	i = 0;
	while (i < p->nbr_segments)
	{
		res = build_command(p, i, envp);
		if (res == -1)
		{
			cleanup_commands(p, i);
			return (0);
		}
		i++;
	}
	return (1);
}

void	execute_pipeline(char **envp, char **segments)
{
	t_pipeline	pipeline;

	if (!init_pipeline(&pipeline, envp, segments))
		return ;
	if (!setup_commands(&pipeline, envp))
		return ;
	pipeline.pids = ft_calloc(pipeline.nbr_segments, sizeof(pid_t));
	if (!pipeline.pids)
	{
		cleanup_commands(&pipeline, pipeline.nbr_segments);
		g_exit_code = 1;
		return ;
	}
	pipeline_loop(&pipeline);
	wait_for_all(pipeline.pids, pipeline.nbr_segments);
	cleanup_commands(&pipeline, pipeline.nbr_segments);
	free(pipeline.pids);
}
