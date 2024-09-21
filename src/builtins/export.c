#include "../../include/minishell.h"

t_ecode	export_builtin(t_shell *shell, char **cmd_args)
{
	char	*key;
	char	*value;
	t_ecode	status;

	if (!cmd_args)
		return (FAILURE);
	if (!cmd_args[1])
		return (declare_builtin(shell, cmd_args));
	key = get_key_from_keyvalue(cmd_args[1]);
	if (!key)
		return (MALLOC_ERROR);
	// printf("KEY: %s\n", key);
	value = NULL;
	status = get_value_from_keyvalue(cmd_args[1], &value);
	if (status != SUCCESS && status != NULL_STRING)
		return (ft_free((void **) &key), MALLOC_ERROR);
	// printf("VALUE: %s\n", value);
	return (update_env_node(&shell->env_list, key, value, true));
}
