/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_prepare.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 00:00:00 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/30 00:00:00 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"
#include <fcntl.h>

static t_token	**parse_segment(char *segment, char ***envp)
{
	t_token	**cmd;

	cmd = tokenize_command(segment, ' ', *envp);
	if (!cmd || !cmd[0])
	{
		if (cmd)
			free_tokens(cmd);
		return (NULL);
	}
	return (cmd);
}

static int	apply_redirs(t_token ***cmd, char ***envp, int *in_fd, int *out_fd)
{
	t_token	**clean;

	clean = handle_redirections(*cmd, *envp, in_fd, out_fd);
	if (!clean)
	{
		*cmd = NULL;
		if (g_exit_code == 2)
			return (-1);
		return (1);
	}
	if (!clean[0])
	{
		free_tokens(clean);
		*cmd = NULL;
		return (1);
	}
	*cmd = clean;
	return (0);
}

static void	reset_fds(int *in_fd, int *out_fd)
{
	if (*in_fd != STDIN_FILENO)
	{
		close(*in_fd);
		*in_fd = STDIN_FILENO;
	}
	if (*out_fd != STDOUT_FILENO)
	{
		close(*out_fd);
		*out_fd = STDOUT_FILENO;
	}
}

int	is_folder(char *arg)
{
	int	fd_test;

	fd_test = open(arg, O_DIRECTORY);
	if (fd_test != -1)
	{
		close(fd_test);
		return (126);
	}
	return (0);
}

t_token	**prepare_command(char *segment, int *in_fd, int *out_fd, char ***envp)
{
	t_token	**cmd;
	int		ret;

	cmd = parse_segment(segment, envp);
	if (!cmd)
		return (NULL);
	ret = apply_redirs(&cmd, envp, in_fd, out_fd);
	if (ret != 0)
	{
		reset_fds(in_fd, out_fd);
		if (ret == -1)
			return ((t_token **)-1);
		return (NULL);
	}
	return (cmd);
}
