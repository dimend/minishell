/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 16:43:05 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/02 16:43:06 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

static int	handle_redirection_logic(t_token **cmd, t_redir_ctx *ctx,
		t_redir_iter *it)
{
	char	*filename;
	int		quoted;
	int		res;

	res = fetch_target(cmd, it->i, &filename, &quoted);
	if (res <= 0)
		return (res);
	res = dispatch_redirection(cmd[it->i]->type, filename, quoted, ctx);
	if (res == -1)
		return (-1);
	remove_consumed_tokens(cmd, &it->i, filename != NULL);
	return (1);
}

static int	process_token(t_token **cmd, t_token **clean, t_redir_iter *it,
		t_redir_ctx *ctx)
{
	int	res;

	res = handle_redirection_logic(cmd, ctx, it);
	if (res == -1)
		return (-1);
	if (res == 0)
	{
		clean[it->j] = cmd[it->i];
		cmd[it->i] = NULL;
		it->j++;
		it->i++;
	}
	return (0);
}

static int	process_each_token(t_token **cmd, t_token **clean, t_redir_iter *it,
		t_redir_ctx *ctx)
{
	while (it->i < it->count && cmd[it->i])
	{
		if (process_token(cmd, clean, it, ctx) == -1)
			return (-1);
	}
	return (0);
}

t_token	**handle_redirections(t_token **cmd, char **envp, int *in_fd,
		int *out_fd)
{
	t_redir_iter	it;
	t_redir_ctx		ctx;
	t_token			**clean;

	clean = init_clean_array(cmd, &it.count);
	if (!clean)
	{
		free_token_array(cmd, it.count);
		return (NULL);
	}
	it.i = 0;
	it.j = 0;
	ctx.envp = envp;
	ctx.in_fd = in_fd;
	ctx.out_fd = out_fd;
	if (process_each_token(cmd, clean, &it, &ctx) == -1)
	{
		free_token_array(cmd, it.count);
		free_token_array(clean, it.j);
		return (NULL);
	}
	return (finalize_clean_array(cmd, clean, it.j));
}
