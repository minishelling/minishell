/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/31 13:32:10 by lprieri       #+#    #+#                 */
/*   Updated: 2024/10/31 16:42:37 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_ecode	export_node(t_shell *shell, char *current_arg);
static bool		is_valid_identifier(char *keyval);

/**
 * @brief Adds given key-value combinations to the environment node.
 * 
 * If there's no arguments passed, it runs the declare built-in,
 * which prints the full environment list in a formatted way.
 * If there's at least an argument, it will attempt to add it
 * to the environment list.
 * It will continue to do so until it runs out of arguments.
 * 
 * @param shell A pointer to the shell structure.
 * @param cmd_args The array of arguments of the given command.
 * 
 * @return If there was a malloc failure it returns immediately
 * with the error code MALLOC_ERROR.
 * If there was any other error it will raise a flag, but it will
 * keep exporting new nodes until it runs out of arguments
 * and at the end it will return FAILURE.
 * Else it returns SUCCESS.
 */
t_ecode	export_builtin(t_shell *shell, char **cmd_args)
{
	t_ecode	status;
	size_t	i;
	bool	failure_flag;

	if (!cmd_args)
		return (FAILURE);
	if (!cmd_args[1])
		return (declare_builtin(shell, cmd_args));
	i = 1;
	failure_flag = false;
	while (cmd_args[i])
	{
		status = export_node(shell, cmd_args[i]);
		if (status == MALLOC_ERROR)
			return (MALLOC_ERROR);
		else if (failure_flag == false && status)
			failure_flag = true;
		i++;
	}
	if (failure_flag)
		return (FAILURE);
	return (SUCCESS);
}

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
 * @param current_arg The current key-val string to add to the env list.
 * 
 * @return 0 if it was possible to update or create the environment node.
 */
static t_ecode	export_node(t_shell *shell, char *current_arg)
{
	char	*key;
	char	*value;
	t_ecode	status;

	if (!is_valid_identifier(current_arg))
	{
		handle_builtin_err("export", current_arg, "not a valid identifier");
		return (FAILURE);
	}
	key = get_key_from_keyvalue(current_arg);
	if (!key)
		return (MALLOC_ERROR);
	value = NULL;
	status = get_value_from_keyvalue(current_arg, &value);
	if (status != SUCCESS && status != NULL_STRING)
		return (ft_free((void **) &key), MALLOC_ERROR);
	status = update_env_node(&shell->env_list, key, value, true);
	ft_free((void **) &key);
	if (value)
		ft_free((void **) &value);
	if (status && status != MALLOC_ERROR)
		return (FAILURE);
	return (status);
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
