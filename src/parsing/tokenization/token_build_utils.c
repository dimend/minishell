/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_build_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 16:42:22 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/02 19:39:43 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "minishell.h"

char	*next_substring(const char **s, char c, int *error)
{
	size_t	len;
	char	*substr;

	while (is_delim(**s, c))
		(*s)++;
	if (!**s)
		return (NULL);
	len = next_c(*s, c);
	substr = ft_substr(*s, 0, len);
	if (!substr)
	{
		*error = 1;
		return (NULL);
	}
	*s += len;
	return (substr);
}

void	assign_token_fields(char *substr, int *quoted, int *type)
{
	*quoted = fully_quoted(substr);
	if (!*quoted && (ft_strchr(substr, '"') || ft_strchr(substr, '\'')))
		*quoted = 3;
	*type = 0;
}

int	store_token(t_token **slot, char *substr, int quoted, int type)
{
	*slot = new_token(substr, quoted, type);
	free(substr);
	if (!*slot)
		return (1);
	return (0);
}
