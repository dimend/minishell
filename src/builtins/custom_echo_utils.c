/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_echo_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 00:00:00 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/27 00:00:00 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

char	*normalize_token(const char *str)
{
	char	*tmp;
	int		i;

	tmp = ft_strdup(str);
	if (!tmp)
		return (NULL);
	remove_quotes(tmp);
	i = 0;
	while (tmp[i])
	{
		tmp[i] = ft_tolower((unsigned char)tmp[i]);
		i++;
	}
	return (tmp);
}

int	is_valid_n_flag(const char *s)
{
	int	j;

	if (!s || s[0] != '-')
		return (0);
	j = 1;
	while (s[j] == 'n')
		j++;
	return (s[j] == '\0');
}

int	skip_quoted_n_flags(t_token **arg, int i)
{
	char	*norm;

	while (arg[i])
	{
		norm = normalize_token(arg[i]->str);
		if (norm && norm[0] == 'n' && norm[1] == '\0' && arg[i]->quoted)
		{
			free(norm);
			i++;
		}
		else
		{
			free(norm);
			break ;
		}
	}
	return (i);
}

int	skip_n_flags(t_token **arg, int start)
{
	int		i;
	char	*norm;

	i = start;
	while (arg[i])
	{
		norm = normalize_token(arg[i]->str);
		if (!is_valid_n_flag(norm))
		{
			free(norm);
			break ;
		}
		if (norm[0] == '-' && norm[1] == 'n' && norm[2] == '\0')
		{
			free(norm);
			i = skip_quoted_n_flags(arg, i + 1);
		}
		else
		{
			free(norm);
			i++;
		}
	}
	return (i);
}
