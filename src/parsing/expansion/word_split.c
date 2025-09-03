/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 23:54:13 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/02 18:05:15 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "minishell.h"

static int	expand_each_token(t_token **out, t_token **arr, int *k)
{
	int	i;

	i = 0;
	while (arr && arr[i])
	{
		if (expand_token(out, arr[i], k))
		{
			free_tokens(out);
			free_tokens(arr + i + 1);
			free(arr);
			return (1);
		}
		i++;
	}
	return (0);
}

static t_token	**finalize_result(t_token **out, t_token **arr, int k)
{
	out[k] = NULL;
	free(arr);
	return (out);
}

t_token	**split_expanded_tokens(t_token **arr)
{
	int		total;
	int		k;
	t_token	**out;

	out = allocate_output(arr, &total);
	if (!out)
		return (NULL);
	k = 0;
	if (expand_each_token(out, arr, &k))
		return (NULL);
	return (finalize_result(out, arr, k));
}
