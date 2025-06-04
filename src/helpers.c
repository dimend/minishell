/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:41:07 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/04 21:03:21 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

char *get_env_value(char **envp, const char *name)
{
    int i;
    size_t len;
	
	len = strlen(name);
	i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
            return (envp[i] + len + 1);
        i++;
    }
    return (NULL);
}

char **get_env_path(char **envp, const char *name)
{
    char *value;
	
	value = get_env_value(envp, name);
    if (!value)
        return (NULL);
    return (ft_split(value, ':'));
}

char *get_path(char **envp, char **cmd)
{
    int i;
    char **paths;
    char *finalpath;
    char *tmp;

	i = 0;
	paths = get_env_path(envp, "PATH");
	finalpath = NULL;
	tmp = NULL;
    while (paths && paths[i])
    {
        tmp = ft_strcatrealloc(paths[i], "/");
        if (tmp)
            tmp = ft_strcatrealloc(tmp, cmd[0]);

        if (tmp && access(tmp, F_OK) == 0)
        {
            finalpath = ft_strdup(tmp);
            free(tmp);
            break;
        }
        free(tmp);
        i++;
    }
    return (finalpath);
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
    else if (ft_strncmp(cmd[0], "exit", ft_strlen(cmd[0])) == 0)
    {
        custom_exit(&cmd[1]);
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