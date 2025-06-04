/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:41:07 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/04 18:30:37 by dimendon         ###   ########.fr       */
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

int custom_cd(char **envp, char **args)
{
    char *target_dir;

	target_dir = NULL;
    if (!args[1] || ft_strlen(args[1]) == 0)
    {
        target_dir = get_env_value(envp, "HOME");
        if (!target_dir)
        {
            printf("cd: HOME not set\n");
            return (1);
        }
    }
    else
    {
        target_dir = args[1];
    }
    if (chdir(target_dir) != 0)
	{
		perror("cd");
		return (1);
	}
    return (0);
}

