/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_build.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 23:53:56 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/01 23:53:57 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "minishell.h"

static void	update_quote(char c, char *quote, int *i)
{
	if (!*quote && (c == '\'' || c == '"'))
	{
		*quote = c;
		(*i)++;
	}
	else if (*quote && c == *quote)
	{
		*quote = 0;
		(*i)++;
	}
}

int	handle_special_sequences(t_expand_ctx *ctx)
{
	char	*tmp;

	if (ctx->quote || ctx->str[ctx->i] != '$' || ctx->str[ctx->i + 1] != '"')
		return (0);
	tmp = append_literal(ctx->result, ctx->str, ctx->start, ctx->i);
	if (!tmp)
		return (-1);
	ctx->result = tmp;
	ctx->start = ctx->i + 1;
	ctx->i++;
	update_quote(ctx->str[ctx->i], &ctx->quote, &ctx->i);
	return (1);
}

int	process_regular_char(t_expand_ctx *ctx)
{
	int	status;
	int	prev_i;

	prev_i = ctx->i;
	update_quote(ctx->str[ctx->i], &ctx->quote, &ctx->i);
	if (ctx->i != prev_i)
		return (1);
	if (ctx->str[ctx->i] == '$' && ctx->quote != '\'')
	{
		status = process_dollar(ctx);
		if (status != 0)
			return (status);
	}
	ctx->i++;
	return (1);
}

int	append_remainder(t_expand_ctx *ctx)
{
	char	*tmp;

	tmp = ft_strcatrealloc(ctx->result, ctx->str + ctx->start);
	if (!tmp)
		return (-1);
	ctx->result = tmp;
	return (0);
}

char	*build_expanded_str(char *str, char **envp)
{
	t_expand_ctx	ctx;
	int				status;

	ctx.str = str;
	ctx.envp = envp;
	ctx.quote = 0;
	ctx.result = NULL;
	ctx.i = 0;
	ctx.start = 0;
	while (ctx.str[ctx.i])
	{
		status = handle_special_sequences(&ctx);
		if (status == -1)
			return (NULL);
		if (status == 1)
			continue ;
		status = process_regular_char(&ctx);
		if (status == -1)
			return (NULL);
	}
	if (append_remainder(&ctx) == -1)
		return (NULL);
	return (ctx.result);
}
