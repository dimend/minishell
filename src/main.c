/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:41:29 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/04 14:04:32 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(void)
{
    char *line;

    while (1)
    {
        line = readline("minishell$ ");
        if (line == NULL)
        {
            printf("\nEOF\n");
            break;
        }

		if (*line)
            add_history(line);
			
        printf("Input: %s\n", line);
        free(line);
    }

	write_history(".minishell_history");
    return (0);
}