#include "minishell.h"
#include "../libft/libft.h"

static char *append_literal(char *result, char *str, int start, int i)
{
    char *tmp;

    str[i] = '\0';
    tmp = ft_strcatrealloc(result, str + start);
    str[i] = '$';
    if (!tmp)
    {
        free(result);
        return (NULL);
    }
    return (tmp);
}

static char *expand_var(char *str, int *var_len)
{
    int i = 0;

    while (str[i] != '\0' && ft_isalnum(str[i]))
        i++;
    *var_len = i;
    if (i > 0)
        return (ft_substr(str, 0, i));
    return (NULL);
}

static char *append_expanded_var(char *result, char *str, int *i, char **envp)
{
    int var_len;
    char *var;
    char *value;
    char *tmp;

    var_len = 0;
    var = expand_var(&str[*i + 1], &var_len);
    value = get_env_value(envp, var);
    if (!value)
        value = "";
    tmp = ft_strcatrealloc(result, value);
    free(var);
    if (!tmp)
        return (NULL);
    *i += var_len + 1;
    return (tmp);
}

char *build_expanded_str(char *str, char **envp)
{
    int i;
    int start;
    char *result;

    i = 0;
    start = 0;
    result = NULL;
    while (str[i])
    {
        if (str[i] == '$' && ft_isalnum(str[i + 1]))
        {
            if (!(result = append_literal(result, str, start, i)))
                return (NULL);
            if (!(result = append_expanded_var(result, str, &i, envp)))
                return (NULL);
            start = i;
        }
        else
            i++;
    }
    if (!(result = ft_strcatrealloc(result, str + start)))
        return (NULL);
    return (result);
}