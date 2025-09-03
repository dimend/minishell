/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_build.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 00:00:00 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/02 16:56:05 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	default_command(t_command *cmd)
{
	cmd->in_fd = STDIN_FILENO;
	cmd->out_fd = STDOUT_FILENO;
	cmd->cmd_type = 0;
	cmd->has_heredoc = 0;
}

static void	check_heredoc(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd->tokens[i])
	{
		if (cmd->tokens[i]->type == 2)
			cmd->has_heredoc = 1;
		i++;
	}
}

static int	apply_redirs(t_command *cmd, char **envp)
{
	cmd->tokens = handle_redirections(cmd->tokens, envp, &cmd->in_fd,
			&cmd->out_fd);
	if (!cmd->tokens)
	{
		if (cmd->in_fd != STDIN_FILENO)
			close(cmd->in_fd);
		if (cmd->out_fd != STDOUT_FILENO)
			close(cmd->out_fd);
		return (0);
	}
	return (1);
}

int	build_command(t_pipeline *p, int i, char **envp)
{
	default_command(&p->cmds[i]);
	p->cmds[i].tokens = tokenize_command(p->segments[i], ' ', envp);
	if (!p->cmds[i].tokens)
	{
		g_exit_code = 1;
		return (-1);
	}
	check_heredoc(&p->cmds[i]);
	if (!apply_redirs(&p->cmds[i], envp))
	{
		if (g_exit_code == 2)
			return (-1);
		default_command(&p->cmds[i]);
		return (0);
	}
	if (p->cmds[i].tokens[0])
		p->cmds[i].cmd_type = is_builtin(p->cmds[i].tokens[0]->str);
	return (1);
}
