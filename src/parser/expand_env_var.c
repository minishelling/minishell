#include "../../include/minishell.h"

void		handle_var_sign(t_shell *shell, char **str, char **expanded_str, \
	t_env *env_list);
static void	expand_env_var(char **str, char **expanded_str, t_env *env_list);
static char	*get_variable_name(char *var_start, char *var_end);

/**
 * @brief Handles the variable sign ($) in the input string and performs 
 *        variable expansion or copies it literally if appropriate.
 *
 * This function processes a dollar sign ($) found in the input string.
 * It checks for specific conditions:
 * - If the dollar sign is standalone or preceded by a single quote (e.g.,
 *   the end portion of '$'), it is copied literally to the expanded string.
 * - If it is "$?", it appends the exit code to the expanded string.
 * - Otherwise, it calls expand_env_var to handle environmental variable 
 *   expansion.
 *
 * This function is called in two scenarios: 
 * 1. If it comes from expand_token_str function, and there are no quotes,
 *    with a single $ at the end of the string.
 * 2. If it comes from "handle_dquote" and is the last $ sign 
 *    before a single quote, as in "'$'".
 *
 * @param shell Pointer to the shell structure containing exit code and 
 *              environment list.
 * @param str Pointer to the current position in the string being processed.
 * @param expanded_str Pointer to the string where expanded results are 
 *                     stored.
 * @param env_list Pointer to the environment variable list.
 */
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

/**
 * @brief Expands an environment variable found in the input string.
 *
 * This function processes a portion of the input string starting from a 
 * dollar sign ($) and identifies the name of the environment variable 
 * to be expanded. It retrieves the corresponding value from the 
 * environment list and appends it to the expanded string.
 *
 * If the environment variable name is invalid, memory for the expanded 
 * string is freed. If the dollar sign is followed by another dollar sign, 
 * it appends a literal dollar sign to the expanded string.
 *
 * @param str Pointer to the current position in the string being processed.
 * @param expanded_str Pointer to the string where expanded results are 
 *                     stored.
 * @param env_list Pointer to the environment variable list.
 */
static void	expand_env_var(char **str, char **expanded_str, t_env *env_list)
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

/**
 * @brief Extracts the variable name from the input string.
 *
 * This function identifies and extracts the variable name that starts 
 * with a dollar sign ($) in the input string. It scans the string from 
 * the starting point of the variable name until it finds a character 
 * that is not alphanumeric or an underscore. The extracted variable 
 * name is returned as a new string.
 *
 * If memory allocation for the variable name fails, this function 
 * returns NULL.
 *
 * @param var_start Pointer to the start of the variable name in the 
 *                  input string.
 * @param var_end Pointer to the end of the variable name in the input 
 *                string.
 *
 * @return A pointer to the extracted variable name string, or NULL if 
 *         memory allocation fails.
 */
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
