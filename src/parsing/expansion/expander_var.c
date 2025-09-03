/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_var.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 23:54:05 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/01 23:54:06 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "minishell.h"

static char	*append_var_char(char *name, char c)
{
	char	tmp[2];
	char	*new;

	tmp[0] = c;
	tmp[1] = '\0';
	new = ft_strcatrealloc(name, tmp);
	if (!new)
		return (NULL);
	return (new);
}

static char	*terminate_var_name(char *name, int *i, int j)
{
	*i = j;
	return (name);
}

char	*collect_var_name(char *str, int *i)
{
	int		j;
	char	*name;

	j = *i + 1;
	name = NULL;
	while (str[j])
	{
		if (str[j] == '\'' || str[j] == '"')
			break ;
		if (ft_isalnum(str[j]) || str[j] == '_')
		{
			name = append_var_char(name, str[j]);
			if (!name)
				return (NULL);
			j++;
			continue ;
		}
		break ;
	}
	return (terminate_var_name(name, i, j));
}
