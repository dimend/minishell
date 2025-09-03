/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 17:51:40 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/02 18:15:21 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_quote_state(char c, char *quote)
{
	if (!*quote && (c == '\'' || c == '"'))
		*quote = c;
	else if (*quote && c == *quote)
		*quote = 0;
}

int	has_unclosed_quotes(const char *s)
{
	char	quote;

	quote = 0;
	while (s && *s)
	{
		if (!quote && (*s == '\'' || *s == '"'))
			quote = *s;
		else if (quote && *s == quote)
			quote = 0;
		s++;
	}
	return (quote != 0);
}
