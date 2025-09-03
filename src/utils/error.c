/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 16:45:01 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/02 16:45:02 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

void	error_msg(const char *prefix, const char *msg)
{
	if (prefix && msg)
		fprintf(stderr, "%s: %s\n", prefix, msg);
	else if (msg)
		fprintf(stderr, "%s\n", msg);
}

void	error_perror(const char *context)
{
	if (context)
		perror(context);
	else
		perror("minishell");
}
