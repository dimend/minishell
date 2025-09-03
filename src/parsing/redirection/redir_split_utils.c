/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_split_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 16:42:08 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/02 16:42:09 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "minishell.h"

static int	advance_redirect(char *str, int *i, int *count)
{
	if (str[*i] == '>' && str[*i + 1] == '>')
	{
		(*count)++;
		*i += 2;
		return (1);
	}
	if (str[*i] == '<' && str[*i + 1] == '<')
	{
		(*count)++;
		*i += 2;
		return (1);
	}
	if (str[*i] == '>' || str[*i] == '<')
	{
		(*count)++;
		(*i)++;
		return (1);
	}
	return (0);
}

static int	part_count(t_token *tok)
{
	int	i;
	int	start;
	int	count;
	int	prev;

	i = 0;
	start = 0;
	count = 0;
	if (tok->quoted && tok->quoted != 3)
		return (1);
	while (tok->str[i])
	{
		prev = i;
		if (advance_redirect(tok->str, &i, &count))
		{
			if (prev - start > 0)
				count++;
			start = i;
		}
		else
			i++;
	}
	if (i - start > 0)
		count++;
	return (count);
}

int	total_parts(t_token **arr)
{
	int	total;
	int	i;

	total = 0;
	i = 0;
	while (arr && arr[i])
	{
		total += part_count(arr[i]);
		i++;
	}
	return (total);
}
