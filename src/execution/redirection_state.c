/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_state.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 00:00:00 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/30 00:00:00 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

void	setup_redirections(int in_fd, int out_fd, int *save_in, int *save_out)
{
	*save_in = -1;
	*save_out = -1;
	if (in_fd != STDIN_FILENO)
	{
		*save_in = dup(STDIN_FILENO);
		if (*save_in == -1)
			error_perror("dup");
		if (dup2(in_fd, STDIN_FILENO) == -1)
			error_perror("dup2");
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		*save_out = dup(STDOUT_FILENO);
		if (*save_out == -1)
			error_perror("dup");
		if (dup2(out_fd, STDOUT_FILENO) == -1)
			error_perror("dup2");
		close(out_fd);
	}
}

void	restore_redirections(int save_in, int save_out)
{
	if (save_in != -1)
	{
		if (dup2(save_in, STDIN_FILENO) == -1)
			error_perror("dup2");
		close(save_in);
	}
	if (save_out != -1)
	{
		if (dup2(save_out, STDOUT_FILENO) == -1)
			error_perror("dup2");
		close(save_out);
	}
}

short int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit"));
}
