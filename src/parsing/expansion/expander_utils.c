/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrandon <kbrandon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 23:54:03 by kbrandon          #+#    #+#             */
/*   Updated: 2025/09/01 23:54:04 by kbrandon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "minishell.h"

char	*append_literal(char *result, char *str, int start, int i)
{
	char	*tmp;

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

char	*expand_var(char *str, int *var_len)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	*var_len = i;
	if (i > 0)
		return (ft_substr(str, 0, i));
	return (NULL);
}

char	*append_expanded_var(char *result, char *str, int *i, char **envp)
{
	int		var_len;
	char	*var;
	char	*value;
	char	*tmp;

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
