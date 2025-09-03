/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 23:54:07 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/01 23:54:08 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define SPACE_MARK 1

void	mark_quoted_spaces(char *str)
{
	char	quote;
	size_t	i;

	quote = 0;
	i = 0;
	while (str[i])
	{
		if (!quote && (str[i] == '\'' || str[i] == '"'))
			quote = str[i];
		else if (quote && str[i] == quote)
			quote = 0;
		else if (quote && str[i] == ' ')
			str[i] = SPACE_MARK;
		i++;
	}
}

void	remove_quotes(char *str)
{
	char	quote;
	size_t	i;
	size_t	j;

	mark_quoted_spaces(str);
	quote = 0;
	i = 0;
	j = 0;
	while (str[i])
	{
		if (!quote && (str[i] == '\'' || str[i] == '"'))
			quote = str[i++];
		else if (quote && str[i] == quote)
		{
			quote = 0;
			i++;
		}
		else
			str[j++] = str[i++];
	}
	str[j] = '\0';
}

void	restore_marked_spaces(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == SPACE_MARK)
			str[i] = ' ';
		i++;
	}
}
