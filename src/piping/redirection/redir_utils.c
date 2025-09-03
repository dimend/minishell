/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 16:42:58 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/03 17:08:47 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

static void	print_syntax_error(char *tok)
{
	char	*tmp;
	char	*msg;

	tmp = ft_strjoin("syntax error near unexpected token `", tok);
	if (tmp)
	{
		msg = ft_strjoin(tmp, "'");
		free(tmp);
		if (msg)
		{
			error_msg("minishell", msg);
			free(msg);
		}
		else
			error_msg("minishell", "syntax error near unexpected token");
	}
	else
		error_msg("minishell", "syntax error near unexpected token");
	g_exit_code = 2;
}

static int	fetch_filename(t_token **cmd, int i, char **filename, int *quoted)
{
	*filename = NULL;
	*quoted = 0;
	if (cmd[i + 1] && cmd[i + 1]->type == 0)
	{
		*filename = cmd[i + 1]->str;
		*quoted = (cmd[i + 1]->quoted != 0);
		return (0);
	}
	if (cmd[i + 1])
		print_syntax_error(cmd[i + 1]->str);
	else
		print_syntax_error("newline");
	return (-1);
}

int	fetch_target(t_token **cmd, int i, char **filename, int *quoted)
{
	if (!(cmd[i] && (cmd[i]->type == 1 || cmd[i]->type == 2 || cmd[i]->type == 3
				|| cmd[i]->type == 4)))
		return (0);
	if (fetch_filename(cmd, i, filename, quoted) == -1)
		return (-1);
	return (1);
}

int	dispatch_redirection(int type, char *filename, int quoted, t_redir_ctx *ctx)
{
	if (type == 1)
		return (apply_infile(filename, ctx->in_fd));
	else if (type == 2)
		return (apply_heredoc(filename, quoted, ctx->envp, ctx->in_fd));
	else if (type == 3)
		return (apply_outfile(filename, ctx->out_fd));
	else
		return (apply_append(filename, ctx->out_fd));
}

void	remove_consumed_tokens(t_token **cmd, int *i, int has_filename)
{
	free_token(cmd[*i]);
	cmd[*i] = NULL;
	if (cmd[*i + 1])
	{
		free_token(cmd[*i + 1]);
		cmd[*i + 1] = NULL;
	}
	if (has_filename)
		*i += 2;
	else
		*i += 1;
}
