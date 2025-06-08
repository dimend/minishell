#include "minishell.h"
#include "libft/libft.h"

short int custom_cd(char **envp, char **args)
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

short int custom_exit(char **args)
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
