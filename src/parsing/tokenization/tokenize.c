/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:12:49 by dimendon          #+#    #+#             */
/*   Updated: 2025/09/02 18:15:14 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "minishell.h"

static t_token	**fill_arr_from_string(const char *s, char c)
{
	t_token	**arr;
	int		token_num;

	arr = alloc_token_array(s, c, &token_num);
	if (!arr)
		return (NULL);
	(void)token_num;
	if (populate_tokens(arr, s, c))
	{
		free_tokens(arr);
		return (NULL);
	}
	return (arr);
}

t_token	**tokenize_command(char const *s, char c, char **envp)
{
	t_token	**arr;

	if (!s)
		return (NULL);
	if (has_unclosed_quotes(s))
	{
		error_msg("minishell",
			"unexpected EOF while looking for matching quote");
		g_exit_code = 2;
		return (NULL);
	}
	arr = fill_arr_from_string(s, c);
	if (!arr)
		return (NULL);
	if (expand_all(arr, envp))
	{
		free_tokens(arr);
		return (NULL);
	}
	return (post_process_tokens(arr));
}
