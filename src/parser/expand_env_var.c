#include "../../include/minishell.h"

char	*handle_double_dollar_sign(char *expanded_str, char **str)
{
	char	*temp_str;

	if (**str == '$' && *(*str + 1) == '$')
	{
		temp_str = ft_strjoin(expanded_str, "$");
		free(expanded_str);
		if (!temp_str)
			return (NULL);
		expanded_str = temp_str;
		(*str)++;
	}
	return (expanded_str);
}

char	*expand_with_env_value(char *expanded_str, char *env_value)
{
	char	*temp_str;

	if (env_value)
	{
		temp_str = ft_strjoin(expanded_str, env_value);
		free(expanded_str);
		if (!temp_str)
			return (NULL);
		expanded_str = temp_str;
	}
	return (expanded_str);
}

static char	*get_variable_name(char *var_start, char *var_end)
{
	while (ft_isalnum(*var_end) || *var_end == '_')
		var_end++;
	return (ft_substr(var_start + 1, 0, var_end - var_start - 1));
}

char	*expand_env_var (char **str, char *expanded_str, t_env *env_list)
{
	char	*var_start;
	char	*var_end;
	char	*var_name;
	char	*env_value;
	
	var_start = *str;
	var_end = var_start + 1;
	while (ft_isalnum(*var_end) || *var_end == '_')
		var_end++;
	var_name = get_variable_name (var_start, var_end);
	if (!var_name)
		return(free(expanded_str), NULL);
	env_value = get_env_value_from_key(env_list, var_name);
	free(var_name);
	expanded_str = expand_with_env_value(expanded_str, env_value);
	if(!expanded_str)
		return(NULL);
	expanded_str = handle_double_dollar_sign(expanded_str, str);
	if (!expanded_str)
		return (NULL);
	*str = var_end;
	return (expanded_str);
}

char	*handle_env_var_sign(t_shell *shell, char **str, char *expanded_str, t_env *env_list)
{	
	if (ft_strncmp(*str, "$", 1) == 0 && ft_strlen(*str) == 1)
	{
		expanded_str = ft_strjoin(expanded_str, "$");
		(*str)++;
		if (!expanded_str)
		return (NULL);
	}
	else if (ft_strncmp(*str, "$?", 2) == 0)
	{
		expanded_str = ft_strjoin(expanded_str, ft_itoa(shell->exit_code));
		(*str) += 2;
		if (!expanded_str)
		return (NULL);
	}
	else
		expanded_str = expand_env_var(str, expanded_str, env_list);
	return (expanded_str);
}
