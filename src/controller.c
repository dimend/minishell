/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:22:38 by dimendon          #+#    #+#             */
/*   Updated: 2025/07/08 17:01:03 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <fcntl.h>

static int	is_folder(char *arg)
{
	int	fd_test;
    
	fd_test = open(arg, O_DIRECTORY);
	if (fd_test != -1)
	{
		close(fd_test);
		return (126);
	}
	return (0);
}

int run_builtin(char ***envp, char **cmd)
{
    if (!ft_strncmp(cmd[0], "echo", 5))
        last_exit_code = custom_echo(cmd);
    else if (!ft_strncmp(cmd[0], "cd", 3))
        last_exit_code = custom_cd(envp, cmd);
    else if (!ft_strncmp(cmd[0], "pwd", 4))
        last_exit_code = custom_pwd();
    else if (!ft_strncmp(cmd[0], "export", 7))
        last_exit_code = custom_export(envp, cmd);
    else if (!ft_strncmp(cmd[0], "unset", 6))
        last_exit_code = custom_unset(envp, cmd);
    else if (!ft_strncmp(cmd[0], "env", 4))
        last_exit_code = custom_env(*envp);
    else if (!ft_strncmp(cmd[0], "exit", 5))
    {
        last_exit_code = custom_exit(cmd);
        if (last_exit_code != 1)
            exit(last_exit_code);
    }
    else
        last_exit_code = 127;

    return (last_exit_code);
}

static void run_single(char ***envp, char *segment)
{
    char **cmd;
    char *path;
    int in_fd;
    int out_fd;
    int save_in;
    int save_out;

    in_fd = STDIN_FILENO;
    out_fd = STDOUT_FILENO;
    cmd = tokenize_command(segment, ' ', *envp);
    if (!cmd)
        return ;
    cmd = handle_redirections(cmd, &in_fd, &out_fd);
    if (!cmd || !cmd[0])
    {
        free_cmd(cmd);
        return ;
    }
    save_in = dup(STDIN_FILENO);
    save_out = dup(STDOUT_FILENO);
    if (in_fd != STDIN_FILENO)
    {
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
    }
    if (out_fd != STDOUT_FILENO)
        dup2(out_fd, STDOUT_FILENO);

    if (is_builtin(cmd[0]))
        run_builtin(envp, cmd);
    else
    {
        if (ft_strchr(cmd[0], '/'))
        {
            if (access(cmd[0], F_OK) != 0)
            {
                perror(cmd[0]);
                last_exit_code = 127;
            }
            else if (is_folder(cmd[0]))
            {
                fprintf(stderr, "%s: Is a directory\n", cmd[0]);
                last_exit_code = 126;
            }
            else if (access(cmd[0], X_OK) != 0)
            {
                perror(cmd[0]);
                last_exit_code = 126;
            }
            else
            {
                last_exit_code = execute_command(cmd[0], cmd, *envp);
            }
        }
        else
        {
            path = get_path(*envp, cmd);
            if (path)
            {
                last_exit_code = execute_command(path, cmd, *envp);
                free(path);
            }
            else
            {
                fprintf(stderr, "%s: command not found\n", cmd[0]);
                last_exit_code = 127;
            }
        }
    }

    if (out_fd != STDOUT_FILENO)
    {
        close(out_fd);
        dup2(save_out, STDOUT_FILENO);
    }
    if (in_fd != STDIN_FILENO)
        dup2(save_in, STDIN_FILENO);
    close(save_in);
    close(save_out);

    free_cmd(cmd);
}

short int is_builtin(const char *cmd)
{
    return (
        ft_strncmp(cmd, "echo", 5) == 0 ||
        ft_strncmp(cmd, "cd", 3) == 0 ||
        ft_strncmp(cmd, "pwd", 4) == 0 ||
        ft_strncmp(cmd, "export", 7) == 0 ||
        ft_strncmp(cmd, "unset", 6) == 0 ||
        ft_strncmp(cmd, "env", 4) == 0 ||
        ft_strncmp(cmd, "exit", 5) == 0
    );
}

void process_command(char ***envp, char *line)
{
    char **segments;
    int count;

    segments = split_pipes(line);
    if (!segments)
        return ;
    count = count_strings(segments);
    if (count > 1)
        execute_pipeline(*envp, segments);
    else
        run_single(envp, segments[0]);
    free_cmd(segments);
}
