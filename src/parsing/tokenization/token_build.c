/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_build.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 16:42:26 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/02 18:15:05 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "minishell.h"

t_token	**alloc_token_array(const char *s, char c, int *token_num)
{
	t_token	**arr;

	*token_num = token_count(s, c);
	arr = malloc(sizeof(t_token *) * (*token_num + 1));
	if (!arr)
		return (NULL);
	return (arr);
}

int	populate_tokens(t_token **arr, const char *s, char c)
{
	int		i;
	int		error;
	char	*substr;
	int		quoted;
	int		type;

	i = 0;
	error = 0;
	while (*s && !error)
	{
		substr = next_substring(&s, c, &error);
		if (!substr)
			break ;
		assign_token_fields(substr, &quoted, &type);
		if (store_token(&arr[i], substr, quoted, type))
			return (1);
		i++;
	}
	if (error)
	{
		arr[i] = NULL;
		return (1);
	}
	arr[i] = NULL;
	return (0);
}

t_token	*new_token(const char *str, int quoted, int type)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->str = ft_strdup(str);
	if (!tok->str)
	{
		free(tok);
		return (NULL);
	}
	tok->quoted = quoted;
	tok->type = type;
	return (tok);
}
