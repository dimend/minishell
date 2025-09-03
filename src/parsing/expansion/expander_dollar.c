/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_dollar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 23:54:01 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/01 23:54:02 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "minishell.h"

int	handle_exit_code_case(t_expand_ctx *ctx)
{
	char	*tmp;
	char	*code;

	if (!(ctx->str[ctx->i] == '$' && ctx->str[ctx->i + 1] == '?'))
		return (0);
	tmp = append_literal(ctx->result, ctx->str, ctx->start, ctx->i);
	if (!tmp)
		return (-1);
	ctx->result = tmp;
	code = ft_itoa(g_exit_code);
	if (!code)
		return (-1);
	tmp = ft_strcatrealloc(ctx->result, code);
	free(code);
	if (!tmp)
		return (-1);
	ctx->result = tmp;
	ctx->i += 2;
	ctx->start = ctx->i;
	return (1);
}

int	expand_env_variable(t_expand_ctx *ctx)
{
	char	*tmp;
	char	*name;
	char	*value;

	if (!ft_isalnum(ctx->str[ctx->i + 1]) && ctx->str[ctx->i + 1] != '_')
		return (0);
	tmp = append_literal(ctx->result, ctx->str, ctx->start, ctx->i);
	if (!tmp)
		return (-1);
	ctx->result = tmp;
	name = collect_var_name(ctx->str, &ctx->i);
	if (!name)
		return (-1);
	value = get_env_value(ctx->envp, name);
	free(name);
	if (!value)
		value = "";
	tmp = ft_strcatrealloc(ctx->result, value);
	if (!tmp)
		return (-1);
	ctx->result = tmp;
	ctx->start = ctx->i;
	return (1);
}

int	process_dollar(t_expand_ctx *ctx)
{
	int	status;

	status = handle_exit_code_case(ctx);
	if (status != 0)
		return (status);
	status = expand_env_variable(ctx);
	if (status != 0)
		return (status);
	return (0);
}
