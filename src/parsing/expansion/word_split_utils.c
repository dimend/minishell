/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_split_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 23:54:11 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/02 18:05:41 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "minishell.h"

static int	compute_total(t_token **arr, int *total)
{
	int		i;
	char	**parts;

	i = 0;
	*total = 0;
	while (arr && arr[i])
	{
		if (arr[i]->quoted)
			(*total)++;
		else
		{
			parts = split_whitespace(arr[i]->str);
			if (!parts)
				return (free_tokens(arr), 1);
			*total += count_strings(parts);
			free_cmd(parts);
		}
		i++;
	}
	return (0);
}

static int	handle_quoted(t_token **out, t_token *tok, int *k)
{
	restore_marked_spaces(tok->str);
	out[(*k)++] = tok;
	return (0);
}

static int	split_unquoted_parts(t_token **out, t_token *tok, int *k)
{
	char	**parts;
	int		j;

	parts = split_whitespace(tok->str);
	free(tok->str);
	free(tok);
	if (!parts)
		return (1);
	j = 0;
	while (parts[j])
	{
		out[(*k)] = new_token(parts[j], 0, 0);
		if (!out[(*k)])
			return (free_cmd(parts), 1);
		restore_marked_spaces(out[(*k)++]->str);
		j++;
	}
	free(parts);
	return (0);
}

int	expand_token(t_token **out, t_token *tok, int *k)
{
	if (tok->quoted)
		return (handle_quoted(out, tok, k));
	return (split_unquoted_parts(out, tok, k));
}

t_token	**allocate_output(t_token **arr, int *total)
{
	t_token	**out;

	if (compute_total(arr, total))
		return (NULL);
	out = malloc(sizeof(t_token *) * (*total + 1));
	if (!out)
		return (free_tokens(arr), NULL);
	return (out);
}
