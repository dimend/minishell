/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:41:38 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/04 21:03:14 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

void execute_command(char *path, char **cmd, char **envp)
{
    pid_t pid;
    int status;

    pid = fork();
    status = 0;
    if (pid == 0)
    {
        if (execve(path, cmd, envp) == -1)
        {
            perror("execve");
            _exit(1);
        }
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
    }
    else
    {
        perror("fork");
    }
}

int custom_cd(char **envp, char **args)
{
    char *target_dir;

	target_dir = NULL;
    if (!args[1] || ft_strlen(args[1]) == 0)
    {
        target_dir = get_env_value(envp, "HOME");
        if (!target_dir)
        {
            printf("cd: HOME not set\n");
            return (1);
        }
    }
    else
    {
        target_dir = args[1];
    }
    if (chdir(target_dir) != 0)
	{
		perror("cd");
		return (1);
	}
    return (0);
}

int custom_exit(char **args)
{
    int status;
    int num_len;

    status = 0;
    if (!args || !args[0])
    {
        printf("exit\n");
        exit(status);
    }
    num_len = ft_is_str_num(args[0]);
    if (num_len == 0 || num_len > 3) {
        printf("exit: %s: numeric argument required\n", args[0]);
        exit(255);
    }
    status = ft_atoi(args[0]);
    if (status < 0 || status > 255) {
        printf("exit\n");
        exit(255);
    }
    printf("exit\n");
    exit(status);
    return 0;
}

