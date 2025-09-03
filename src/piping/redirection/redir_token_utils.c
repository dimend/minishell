/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_token_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 16:43:02 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/02 16:43:03 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

void	free_token(t_token *tok)
{
	if (!tok)
		return ;
	free(tok->str);
	free(tok);
}

void	free_token_array(t_token **arr, int count)
{
	int	k;

	k = 0;
	if (!arr)
		return ;
	while (k < count)
	{
		if (arr[k])
			free_token(arr[k]);
		k++;
	}
	free(arr);
}

int	count_tokens(t_token **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
		i++;
	return (i);
}

t_token	**alloc_clean_array(int count)
{
	t_token	**clean;

	clean = malloc(sizeof(t_token *) * (count + 1));
	if (!clean)
		return (NULL);
	return (clean);
}
