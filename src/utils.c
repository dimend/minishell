/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:41:38 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/04 18:15:56 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

void execute_command(char *path, char **cmd, char **envp)
{
    pid_t pid;
    int status;

    pid = fork();
    status = 0;
    if (pid == 0)
    {
        if (execve(path, cmd, envp) == -1)
        {
            perror("execve");
            _exit(1);
        }
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
    }
    else
    {
        perror("fork");
    }
}

void process_command(char **envp, char *line)
{
    char **cmd;
    char *path;

    cmd = ft_split(line, ' ');
    path = NULL;

    if (!cmd || !cmd[0])
    {
        free_cmd(cmd);
        return;
    }
    
    if (ft_strncmp(cmd[0], "cd", ft_strlen(cmd[0])) == 0)
    {
        custom_cd(envp, cmd);
    }
    else
    {
        path = get_path(envp, cmd);
        if (path)
        {
            execute_command(path, cmd, envp);
            free(path);
        }
        else
        {
            printf("Command not found: %s\n", cmd[0]);
        }
    }
    free_cmd(cmd);
}

