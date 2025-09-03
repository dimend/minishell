/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whitespace_split.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 23:54:09 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/02 18:15:43 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "minishell.h"

static int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

static int	count_words(const char *s)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (is_whitespace(s[i]))
			i++;
		if (!s[i])
			break ;
		count++;
		while (s[i] && !is_whitespace(s[i]))
			i++;
	}
	return (count);
}

static void	fill_words(char **out, const char *s)
{
	int	i;
	int	k;
	int	start;

	i = 0;
	k = 0;
	while (s[i])
	{
		while (is_whitespace(s[i]))
			i++;
		if (!s[i])
			break ;
		start = i;
		while (s[i] && !is_whitespace(s[i]))
			i++;
		out[k++] = ft_substr(s, start, i - start);
	}
	out[k] = NULL;
}

char	**split_whitespace(const char *s)
{
	char	**out;
	int		count;

	count = count_words(s);
	out = malloc(sizeof(char *) * (count + 1));
	if (!out)
		return (NULL);
	fill_words(out, s);
	return (out);
}
