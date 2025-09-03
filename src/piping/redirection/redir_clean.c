/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_clean.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 16:43:09 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/02 16:43:10 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

t_token	**init_clean_array(t_token **cmd, int *count)
{
	*count = count_tokens(cmd);
	return (alloc_clean_array(*count));
}

t_token	**finalize_clean_array(t_token **cmd, t_token **clean, int j)
{
	clean[j] = NULL;
	free(cmd);
	if (j == 0)
	{
		free_token_array(clean, j);
		return (NULL);
	}
	return (clean);
}
