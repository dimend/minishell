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
    long long value;

    printf("exit\n");

    if (!args || !args[1])
        exit(0);

    if (!ft_atoany(args[1], &value))
    {
        printf("exit: %s: numeric argument required\n", args[1]);
        exit(255);
    }
    if (args[2])
    {
        printf("exit: too many arguments\n");
        return (1);
    }
    exit((unsigned char)(value % 256));
}

short int custom_echo(char **arg)
{
    int i;
    int flag_newline;

    i = 1;
    flag_newline = 1;
    if (arg[1] && ft_strncmp(arg[1], "-n", 2) == 0)
    {
        flag_newline = 0;
        i++;
    }
    while (arg[i])
    {
        if (write(1, arg[i], ft_strlen(arg[i])) == -1)
        {
            perror("Command failed");
            return (1);
        }
        if (arg[i + 1] != NULL)
        {
            if (write(1, " ", 1) == -1)
            {
                perror("Command failed");
                return (1);
            }
        }
        i++;
    }
    if (flag_newline) {
        write(1, "\n", 1);
    }
    return (0);
}

short int custom_pwd(char **arg)
{
    char *path;
    int logical;
    
    logical = 1;
    if(arg[1] != NULL && ft_strncmp(arg[1], "-P", 2) == 0)
            logical = 0;

    if (logical)
    {
        path = getenv("PWD");    
        if (!path)
        {
            perror("PWD missing, using physical path");
            logical = 0;
        }
    }
    if (!logical)
    {
        path = getcwd(NULL, 0);
        if (!path)
        {
            perror("getcwd failed");
            return (1);
        }
    }
    printf("%s\n", path);
    if (!logical)
        free(path);

    return(0);
}
