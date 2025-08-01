/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kievaughn <kievaughn@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:51:10 by dimendon          #+#    #+#             */
/*   Updated: 2025/07/01 17:29:17 by kievaughn        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/libft.h"

static void remove_var(char ***envp, const char *var)
{
    int i;
    int pos;

    i = 0;
    pos = 0;
    while ((*envp)[i])
    {
    if (ft_strncmp((*envp)[i], var, ft_strlen(var)) == 0
        && (*envp)[i][ft_strlen(var)] == '=')
        {
            free((*envp)[i]);
            pos = i;
            while ((*envp)[pos + 1])
            {
                (*envp)[pos] = (*envp)[pos + 1];
                pos++;
            }
            (*envp)[pos] = NULL;
        }
        else
            i++;
    }
}

short int custom_unset(char ***envp, char **args)
{
    int i;

    i = 1;
    while (args[i])
    {
        remove_var(envp, args[i]);
        i++;
    }
    return 0;
}
