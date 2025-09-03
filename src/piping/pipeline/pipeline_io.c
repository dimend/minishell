/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_io.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 16:00:00 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/26 16:00:00 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_input_redirection(int redir_in, int *in_fd)
{
	if (redir_in != STDIN_FILENO)
	{
		if (redir_in < 0)
		{
			error_perror("Input redirection");
			exit(1);
		}
		dup2(redir_in, STDIN_FILENO);
		close(redir_in);
		if (*in_fd != STDIN_FILENO)
		{
			close(*in_fd);
			*in_fd = STDIN_FILENO;
		}
	}
	else if (*in_fd != STDIN_FILENO)
	{
		dup2(*in_fd, STDIN_FILENO);
		close(*in_fd);
	}
}

void	handle_output_redirection(int redir_out, int *fd, int last)
{
	if (!last && redir_out == STDOUT_FILENO)
	{
		dup2(fd[1], STDOUT_FILENO);
	}
	else if (redir_out != STDOUT_FILENO)
	{
		if (redir_out < 0)
		{
			error_perror("Output redirection");
			exit(1);
		}
		dup2(redir_out, STDOUT_FILENO);
	}
}
