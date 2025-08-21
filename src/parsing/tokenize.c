/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:12:49 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/21 15:23:13 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

static size_t   skip_token(char const *s, size_t i, char c)
{
        char    quote;

        quote = 0;
        while (s[i])
        {
                if (!quote && (s[i] == '"' || s[i] == '\''))
                {
                        quote = s[i++];
                        while (s[i] && s[i] != quote)
                                i++;
                        if (s[i] == quote)
                                i++;
                        quote = 0;
                        continue ;
                }
                if (!quote && s[i] == c)
                        break ;
                i++;
        }
        return (i);
}

static size_t   token_count(char const *s, char c)
{
        size_t  i;
        size_t  count;

        i = 0;
        count = 0;
        while (s[i])
        {
                while (s[i] == c)
                        i++;
                if (!s[i])
                        break ;
                count++;
                i = skip_token(s, i, c);
        }
        return (count);
}

static size_t   next_c(char *s, char c)
{
        size_t  len;
        char    quote;

        len = 0;
        quote = 0;
        while (s[len])
        {
                if (!quote && (s[len] == '"' || s[len] == '\''))
                {
                        quote = s[len++];
                        while (s[len] && s[len] != quote)
                                len++;
                        if (s[len] == quote)
                                len++;
                        quote = 0;
                        continue ;
                }
                if (!quote && s[len] == c)
                        break ;
                len++;
        }
        return (len);
}

static int	fill_arr_from_string(char const *s, char c,
			char **arr, int *quoted, char **envp)
{
	int		i;
	size_t	len;
	char	*expanded;

	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (!*s)
			break ;
		len = next_c((char *)s, c);
		arr[i] = ft_substr((char *)s, 0, len);
		if (!arr[i])
			return (-1);
		if (arr[i][0] == '\'' && arr[i][ft_strlen(arr[i]) - 1] == '\'')
			quoted[i] = 1;
		else if (arr[i][0] == '"' && arr[i][ft_strlen(arr[i]) - 1] == '"')
			quoted[i] = 2;
		else
			quoted[i] = 0;
		if (quoted[i] != 1)
		{
			expanded = build_expanded_str(arr[i], envp);
			free(arr[i]);
			if (!expanded)
				return (-1);
			arr[i] = expanded;
		}
		remove_quotes(arr[i]);

		i++;
		s += len;
	}
	return (i);
}

char	**tokenize_command(char const *s, char c, char **envp, int **quoted)
{
	char	**arr;
	int		*qt;
	int		token_num;

	if (!s)
		return (NULL);
	token_num = token_count(s, c);
	arr = (char **)malloc((token_num + 1) * sizeof(char *));
	qt = (int *)malloc(token_num * sizeof(int));
	if (!arr || !qt)
		return (free(arr), free(qt), NULL);
	token_num = fill_arr_from_string(s, c, arr, qt, envp);
	if (token_num < 0)
		return (free_cmd(arr), free(qt), NULL);
	arr[token_num] = NULL;

	arr = split_expanded_tokens(arr);
	if (!arr)
		return (free(qt), NULL);
	arr = split_redirs(arr);
	if (!arr)
		return (free(qt), NULL);

	*quoted = qt;
	return (arr);
}

