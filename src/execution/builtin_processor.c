/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_processor.c                               :+:      :+:    :+:   */
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

int	trim_command_name(t_token **cmd)
{
	char	*trimmed;

	if (!cmd || !cmd[0] || !cmd[0]->str)
	{
		g_exit_code = 127;
		return (1);
	}
	trimmed = ft_strtrim(cmd[0]->str, " \t\n\r\v\f");
	if (!trimmed)
	{
		g_exit_code = 127;
		return (1);
	}
	free(cmd[0]->str);
	cmd[0]->str = trimmed;
	return (0);
}

int	dispatch_builtin(char *name, char ***envp, t_token **cmd)
{
	if (!ft_strcmp(name, "echo"))
		g_exit_code = custom_echo(cmd);
	else if (!ft_strcmp(name, "cd"))
		g_exit_code = custom_cd(envp, cmd);
	else if (!ft_strcmp(name, "pwd"))
		g_exit_code = custom_pwd();
	else if (!ft_strcmp(name, "export"))
		g_exit_code = custom_export(envp, cmd);
	else if (!ft_strcmp(name, "unset"))
		g_exit_code = custom_unset(envp, cmd);
	else if (!ft_strcmp(name, "env"))
		g_exit_code = custom_env(*envp, cmd);
	else if (!ft_strcmp(name, "exit"))
	{
		g_exit_code = custom_exit(cmd);
		if (g_exit_code != 1)
			exit(g_exit_code);
	}
	else
		g_exit_code = 127;
	return (g_exit_code);
}

int	run_builtin(char ***envp, t_token **cmd)
{
	char	*name;

	if (trim_command_name(cmd))
		return (g_exit_code);
	name = cmd[0]->str;
	return (dispatch_builtin(name, envp, cmd));
}
