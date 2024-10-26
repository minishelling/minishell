#include "../../include/minishell.h"

static bool	is_valid_identifier(char *keyval);

/**
 * @brief Adds a given key-value combination to the environment node.
 * 
 * It creates a new node if the key didn't exist before,
 * otherwise it updates it with the new value.
 * 
 * If there's at least an equal sign after a key,
 * it adds key, value and keyvalue.
 * Else it only adds the key to the node.
 * 
 * @param shell A pointer to the shell structure.
 * @param cmd_args The array of arguments of the given command.
 * 
 * @return 0 if it was possible to update or create the environment node.
 */
t_ecode	export_builtin(t_shell *shell, char **cmd_args)
{
	char	*key;
	char	*value;
	t_ecode	status;

	if (!cmd_args)
		return (FAILURE);
	if (!cmd_args[1])
		return (declare_builtin(shell, cmd_args));
	if (!is_valid_identifier(cmd_args[1]))
	{
		handle_builtin_err(cmd_args[0], cmd_args[1], "not a valid identifier");
		return (FAILURE);
	}
	key = get_key_from_keyvalue(cmd_args[1]);
	if (!key)
		return (MALLOC_ERROR);
	value = NULL;
	status = get_value_from_keyvalue(cmd_args[1], &value);
	if (status != SUCCESS && status != NULL_STRING)
		return (ft_free((void **) &key), MALLOC_ERROR);
	return (update_env_node(&shell->env_list, key, value, true));
}

/**
 * @brief Checks if a key-value string is a valid identifier to export.
 * 
 * @param keyval A key-value string to be added to the environment if valid.
 * 
 * @return True if the key is a valid identifier, false otherwise.
 */
static bool	is_valid_identifier(char *keyval)
{
	int	i;

	if (keyval && (keyval[0] != '_' && !ft_isalpha(keyval[0])))
		return (false);
	i = 1;
	while (keyval[i])
	{
		if (keyval[i] == '=')
			break ;
		if (keyval[i] != '_' && !ft_isalnum(keyval[i]))
			return (false);
		i++;
	}
	return (true);
}
