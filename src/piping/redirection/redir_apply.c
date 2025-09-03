/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_apply.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 16:43:11 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/02 17:55:12 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

int	apply_infile(char *filename, int *in_fd)
{
	if (!filename || filename[0] == '\0' || open_infile(filename, in_fd) == -1)
		return (-1);
	return (0);
}

int	apply_heredoc(char *filename, int quoted, char **envp, int *in_fd)
{
	const char	*name;

	if (filename)
		name = filename;
	else
		name = "";
	if (handle_heredoc(name, quoted, envp, in_fd) == -1)
		return (-1);
	return (0);
}

int	apply_outfile(char *filename, int *out_fd)
{
	if (!filename || filename[0] == '\0' || open_outfile(filename, out_fd)
		== -1)
		return (-1);
	return (0);
}

int	apply_append(char *filename, int *out_fd)
{
	if (!filename || filename[0] == '\0' || open_appendfile(filename, out_fd)
		== -1)
		return (-1);
	return (0);
}
