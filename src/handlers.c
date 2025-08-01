/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:22:36 by dimendon          #+#    #+#             */
/*   Updated: 2025/07/30 15:22:14 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

int	wait_next(pid_t pid, int *status)
{
	if (waitpid(pid, status, 0) == -1)
	{
		perror("waitpid");
		return (1);
	}
	if (WIFEXITED(*status))
		return (WEXITSTATUS(*status));
	else if (WIFSIGNALED(*status))
		return (128 + WTERMSIG(*status));
	else
		return (1);
}

int	execute_command(char *path, char **cmd, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		execve(path, cmd, envp);
		perror("execve");
		_exit(127);
	}
	else if (pid > 0)
	{
		return (wait_next(pid, &status));
	}
	else
	{
		perror("fork");
		return (1);
	}
}
