#include "../../include/minishell.h"

void		handle_var_sign(t_shell *shell, char **str, char **expanded_str, \
	t_env *env_list);
void		expand_env_var(char **str, char **expanded_str, t_env *env_list);
static char	*get_variable_name(char *var_start, char *var_end);

// If it comes from "decide expansion type" and there are no quotes and a 
// single $ before ending, or if it comes from "handle_dquote" and is the last
// $ sign before a \' as in "'$'"
void	handle_var_sign(t_shell *shell, char **str, char **expanded_str, \
	t_env *env_list)
{
	char	*str_exit_code;

	if ((ft_strncmp(*str, "$", 1) == 0 && ft_strlen(*str) == 1) \
		|| ft_strncmp(*str, "$'", 2) == 0)
	{
		*expanded_str = ft_strjoin_fs1(expanded_str, "$");
		(*str)++;
	}
	else if (ft_strncmp(*str, "$?", 2) == 0)
	{
		str_exit_code = ft_itoa(shell->exit_code);
		if (!str_exit_code)
			ft_free((void **) expanded_str);
		*expanded_str = ft_strjoin_fs1(expanded_str, str_exit_code);
		free(str_exit_code);
		(*str) += 2;
	}
	else
		expand_env_var(str, expanded_str, env_list);
}

void	expand_env_var(char **str, char **expanded_str, t_env *env_list)
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
		ft_free((void **)&expanded_str);
	env_value = get_env_value_from_key(env_list, var_name);
	free(var_name);
	if (env_value)
		*expanded_str = ft_strjoin_fs1(expanded_str, env_value);
	if (**str == '$' && *(*str + 1) == '$')
	{
		*expanded_str = ft_strjoin_fs1(expanded_str, "$");
		(*str)++;
	}
	*str = var_end;
}

static char	*get_variable_name(char *var_start, char *var_end)
{
	char	*var_name;

	while (ft_isalnum(*var_end) || *var_end == '_')
		var_end++;
	var_name = ft_substr(var_start + 1, 0, var_end - var_start - 1);
	if (!var_name)
		return (NULL);
	return (var_name);
}
