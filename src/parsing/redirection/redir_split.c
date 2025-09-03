/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 16:42:11 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/02 18:06:47 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "minishell.h"

static void	copy_literal(t_token *tok, t_token **out, int *idx, t_span span)
{
	char	*part;

	if (span.end - span.start <= 0)
		return ;
	part = ft_substr(tok->str, span.start, span.end - span.start);
	if (!part)
		return ;
	out[(*idx)++] = new_token(part, tok->quoted, 0);
	free(part);
}

static void	emit_operator(const char *str, int *j, t_token **out, int *idx)
{
	if (str[*j] == '>' && str[*j + 1] == '>')
	{
		out[(*idx)++] = new_token(">>", 0, 4);
		*j += 2;
	}
	else if (str[*j] == '<' && str[*j + 1] == '<')
	{
		out[(*idx)++] = new_token("<<", 0, 2);
		*j += 2;
	}
	else if (str[*j] == '>')
	{
		out[(*idx)++] = new_token(">", 0, 3);
		(*j)++;
	}
	else
	{
		out[(*idx)++] = new_token("<", 0, 1);
		(*j)++;
	}
}

static void	process_loop(t_token *tok, t_token **out, int *idx,
		t_split_ctx *ctx)
{
	t_span	span;

	while (tok->str[ctx->j])
	{
		if (!ctx->quote && (tok->str[ctx->j] == '\''
				|| tok->str[ctx->j] == '"'))
			ctx->quote = tok->str[ctx->j];
		else if (ctx->quote && tok->str[ctx->j] == ctx->quote)
			ctx->quote = 0;
		else if (!ctx->quote && (tok->str[ctx->j] == '>'
				|| tok->str[ctx->j] == '<'))
		{
			span.start = ctx->start;
			span.end = ctx->j;
			copy_literal(tok, out, idx, span);
			emit_operator(tok->str, &ctx->j, out, idx);
			ctx->start = ctx->j;
			continue ;
		}
		ctx->j++;
	}
}

static void	split_token(t_token *tok, t_token **out, int *idx)
{
	t_split_ctx	ctx;
	t_span		span;

	ctx.j = 0;
	ctx.start = 0;
	ctx.quote = 0;
	if (tok->quoted && tok->quoted != 3)
	{
		out[(*idx)++] = tok;
		return ;
	}
	process_loop(tok, out, idx, &ctx);
	span.start = ctx.start;
	span.end = ctx.j;
	copy_literal(tok, out, idx, span);
	free(tok->str);
	free(tok);
}

t_token	**split_redirs(t_token **arr)
{
	t_token	**out;
	int		i;
	int		idx;

	out = malloc(sizeof(t_token *) * (total_parts(arr) + 1));
	if (!out)
		return (free_tokens(arr), NULL);
	i = 0;
	idx = 0;
	while (arr && arr[i])
		split_token(arr[i++], out, &idx);
	out[idx] = NULL;
	free(arr);
	return (out);
}
