#include "../../include/minishell.h"

/**
 * @brief Updates a given value by replacing the initial tilde ('~')
 *        with the user's home directory path.
 * 
 * This function checks if the provided value begins with a tilde, 
 * indicating the home directory. If it does, the function replaces 
 * the tilde with the actual home directory path.
 * 
 * @param value A pointer to a string (char **) that potentially begins 
 *              with '~'. On success, it will be replaced by a string
 *              with '~' expanded to the home directory.
 * 
 * @return Returns SUCCESS if the operation is successful, MALLOC_ERROR 
 *         if any memory allocation fails, printing an error message in 
 *         that case.
 */
t_ecode	update_home_value(char **value)
{
	char	*home_value;
	char	*new_value;

	if (value && *value && *value[0] == '~')
	{
		home_value = get_home();
		if (!home_value)
			return (handle_perror("update_env_node"), MALLOC_ERROR);
		new_value = ft_strjoin_fs1(&home_value, &(*value)[1]);
		ft_free((void **) value);
		if (!new_value)
			return (handle_perror("update_env_node"), MALLOC_ERROR);
		*value = ft_strdup(new_value);
		if (!*value)
			return (handle_perror("update_env_node"), MALLOC_ERROR);
		ft_free((void **) &new_value);
	}
	return (SUCCESS);
}
