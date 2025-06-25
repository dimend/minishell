#include "minishell.h"
#include "libft/libft.h"

typedef struct s_pipefd {
    int in_fd;
    int out_fd;
}   t_pipefd;

static int count_segments(char **segments)
{
    int i = 0;
    while (segments && segments[i])
        i++;
    return (i);
}

/* Setup child process: redirect fds and exec command */
static void setup_child_process(char **envp, char **cmd, t_pipefd pipes, int is_last)
{
    if (pipes.in_fd != 0)
    {
        dup2(pipes.in_fd, STDIN_FILENO);
        close(pipes.in_fd);
    }
    if (!is_last)
    {
        dup2(pipes.out_fd, STDOUT_FILENO);
        close(pipes.out_fd);
    }

    if (is_builtin(cmd[0]))
        run_builtin(&envp, cmd);
    else
    {
        char *path = get_path(envp, cmd);
        if (path)
        {
            execve(path, cmd, envp);
            perror("execve");
            free(path);
        }
        else
            fprintf(stderr, "Command not found: %s\n", cmd[0]);
    }
    free_cmd(cmd);
    _exit(1);
}

/* Wait for all children and update global exit code */
static void wait_for_children(pid_t *pids, int num)
{
    int status;
    int j = 0;
    last_exit_code = 0;
    while (j < num)
    {
        if (waitpid(pids[j], &status, 0) == -1)
            perror("waitpid");
        else if (WIFEXITED(status))
            last_exit_code = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            last_exit_code = 128 + WTERMSIG(status);
        j++;
    }
}

void execute_pipeline(char **envp, char **segments)
{
    int     num = count_segments(segments);
    pid_t   *pids = malloc(sizeof(pid_t) * num);
    int     in_fd = 0;   // previous pipe's read end (or stdin for first)
    int     i = 0;

    if (!pids)
        return;

    while (i < num)
    {
        char **cmd = ft_tokenize(segments[i], ' ', envp);
        if (!cmd || !cmd[0])
        {
            free_cmd(cmd);
            i++;
            continue;
        }

        int fd[2] = {-1, -1};
        t_pipefd pipes = {in_fd, -1};

        if (i < num - 1)
        {
            if (pipe(fd) == -1)
            {
                perror("pipe");
                free_cmd(cmd);
                break;
            }
            pipes.out_fd = fd[1];
        }

        pids[i] = fork();
        if (pids[i] == 0)
        {
            // Child
            if (i < num - 1)
            {
                close(fd[0]); // child does not read from the pipe, only writes
            }
            setup_child_process(envp, cmd, pipes, i == num - 1);
        }
        else if (pids[i] < 0)
        {
            perror("fork");
            if (i < num - 1)
            {
                close(fd[0]);
                close(fd[1]);
            }
            free_cmd(cmd);
            break;
        }

        // Parent
        if (in_fd != 0)
            close(in_fd);
        if (i < num - 1)
        {
            close(fd[1]); // parent closes write end, keeps read end for next child
            in_fd = fd[0];
        }
        else
        {
            in_fd = 0;
        }
        free_cmd(cmd);
        i++;
    }
    if (in_fd != 0)
        close(in_fd);

    wait_for_children(pids, num);
    free(pids);
}
