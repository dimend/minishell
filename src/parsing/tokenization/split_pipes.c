/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 17:54:10 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/02 18:14:48 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "minishell.h"

static size_t	count_segments(const char *line)
{
	size_t	i;
	size_t	count;
	char	quote;

	i = 0;
	count = 1;
	quote = 0;
	while (line && line[i])
	{
		if (!quote && (line[i] == '\'' || line[i] == '"'))
			quote = line[i];
		else if (quote && line[i] == quote)
			quote = 0;
		else if (!quote && line[i] == '|')
			count++;
		i++;
	}
	return (count);
}

static char	**pipe_syntax_error(t_pipe_state *st)
{
	while (st->seg-- > 0)
		free(st->arr[st->seg]);
	free(st->arr);
	fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
	g_exit_code = 2;
	return (NULL);
}

static int	handle_pipe_segment(const char *line, size_t i, t_pipe_state *st)
{
	if (i > 0 && line[i - 1] == '>')
	{
		pipe_syntax_error(st);
		return (0);
	}
	if (i == st->start)
	{
		pipe_syntax_error(st);
		return (0);
	}
	st->arr[st->seg++] = ft_substr(line, st->start, i - st->start);
	st->start = i + 1;
	return (1);
}

static int	handle_final_segment(const char *line, size_t i, t_pipe_state *st)
{
	if (st->start == i)
	{
		pipe_syntax_error(st);
		return (0);
	}
	st->arr[st->seg++] = ft_substr(line, st->start, i - st->start);
	return (1);
}

char	**split_pipes(const char *line)
{
	size_t			i;
	char			quote;
	t_pipe_state	st;

	i = 0;
	quote = 0;
	st.start = 0;
	st.seg = 0;
	st.arr = malloc(sizeof(char *) * (count_segments(line) + 1));
	if (!st.arr)
		return (NULL);
	while (line && line[i])
	{
		handle_quote_state(line[i], &quote);
		if (!quote && line[i] == '|' && !handle_pipe_segment(line, i, &st))
			return (NULL);
		i++;
	}
	if (!handle_final_segment(line, i, &st))
		return (NULL);
	st.arr[st.seg] = NULL;
	return (st.arr);
}
