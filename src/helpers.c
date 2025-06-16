/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:41:07 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/16 20:21:02 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

char **copy_envp(char **envp)
{
    char **env;
    int size;
    int i;

    i = -1;
    size = 0;
    while (envp[size])
        size++;
    env = malloc(sizeof(char *) * (size + 1));
    if (!env)
        return (NULL);
    while (++i < size)
    {
        env[i] = ft_strdup(envp[i]);
        if (!env[i])
        {
            while (--i >= 0)
                free(env[i]);
            free(env);
            return (NULL);
        }
    }
    env[size] = NULL;
    return (env);
}

int env_size(char **env)
{
    int size;
    
    size = 0;
    while (env[size])
        size++;
        
    return (size);
}

char **env_realloc_add(char **env)
{
    int size;
    char **new_env;
    int i;
    
    size = env_size(env);
    new_env = malloc(sizeof(char *) * (size + 2));
    i = 0;
    if (!new_env)
        return (NULL);
    while(i < size)
    {
        new_env[i] = env[i];
        i++;
    }
    new_env[size] = NULL;
    free(env);
    return (new_env);
}

int env_add(char ***env_ptr, const char *new_var)
{
    char **env = *env_ptr;
    int env_size = 0;
    char **new_env;

    while (env && env[env_size])
        env_size++;

    new_env = env_realloc_add(env);
    if (!new_env)
        return (-1);
    new_env[env_size] = ft_strdup(new_var);
    if (!new_env[env_size])
    {
        free(new_env);
        return (-1);
    }
    new_env[env_size + 1] = NULL;
    *env_ptr = new_env;
    return (0);
}
