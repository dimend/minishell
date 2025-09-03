/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_post.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 16:42:28 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/02 18:15:10 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "minishell.h"

static int	expand_token_string(t_token *tok, char **envp)
{
	char	*expanded;

	if (tok->quoted == 1)
		return (0);
	expanded = build_expanded_str(tok->str, envp);
	if (!expanded)
		return (1);
	free(tok->str);
	tok->str = expanded;
	return (0);
}

static void	strip_quotes_if_needed(t_token *tok)
{
	if (ft_strchr(tok->str, '"') || ft_strchr(tok->str, '\''))
	{
		remove_quotes(tok->str);
		if (tok->quoted == 3)
			tok->quoted = 3;
	}
}

int	expand_all(t_token **arr, char **envp)
{
	int	i;

	i = 0;
	while (arr && arr[i])
	{
		if (expand_token_string(arr[i], envp))
			return (1);
		strip_quotes_if_needed(arr[i]);
		i++;
	}
	return (0);
}

t_token	**post_process_tokens(t_token **arr)
{
	arr = split_expanded_tokens(arr);
	if (!arr)
		return (NULL);
	arr = split_redirs(arr);
	if (!arr)
		return (NULL);
	return (arr);
}
