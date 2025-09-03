/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 16:45:37 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/02 16:45:42 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"
#include <errno.h>

int	exit_code_from_errno(void)
{
	if (errno == ENOENT)
		return (127);
	return (126);
}

char	**tokens_to_argv(t_token **cmd)
{
	int		count;
	char	**argv;
	int		i;

	count = 0;
	while (cmd && cmd[count])
		count++;
	argv = malloc((count + 1) * sizeof(char *));
	if (!argv)
		return (NULL);
	i = 0;
	while (i < count)
	{
		argv[i] = ft_strdup(cmd[i]->str);
		i++;
	}
	argv[count] = NULL;
	return (argv);
}
