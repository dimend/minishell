/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:22:38 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/16 20:48:30 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

void run_builtin(char ***envp, char **cmd)
{
    if (!ft_strncmp(cmd[0], "cd", 3))
        custom_cd(*envp, cmd);
    else if (!ft_strncmp(cmd[0], "exit", 5))
        custom_exit(cmd);
    else if (!ft_strncmp(cmd[0], "echo", 5))
        custom_echo(cmd);
    else if (!ft_strncmp(cmd[0], "pwd", 4))
        custom_pwd();
    else if (!ft_strncmp(cmd[0], "export", 7))
        custom_export(envp, cmd);
    else if (!ft_strncmp(cmd[0], "exit", 5))
        custom_exit(cmd);
}

short int is_builtin(const char *cmd)
{
    return (
        ft_strncmp(cmd, "echo", 5) == 0 ||
        ft_strncmp(cmd, "cd", 3) == 0 ||
        ft_strncmp(cmd, "pwd", 4) == 0 ||
        ft_strncmp(cmd, "export", 7) == 0 ||
/*        ft_strncmp(cmd, "unset", 6) == 0 ||
        ft_strncmp(cmd, "env", 4) == 0 ||
*/
        ft_strncmp(cmd, "exit", 5) == 0
    );
}

void process_command(char ***envp, char *line)
{
    char **cmd;
    char *path;
    cmd = ft_tokenize(line, ' ');
    
    if (!cmd || !cmd[0])
    {
        free_cmd(cmd);
        return;
    }
    if (is_builtin(cmd[0]))
        run_builtin(envp, cmd);
    else
    {
        path = get_path(*envp, cmd);
        if (path)
        {
            execute_command(path, cmd, *envp);
            free(path);
        }
        else
            printf("Command not found: %s\n", cmd[0]);
    }
    free_cmd(cmd);
}
