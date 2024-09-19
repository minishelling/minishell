#include "../../include/minishell.h"

int	export_builtin(t_shell *shell, char **args)
{
	char	*key;
	char	*value;
	t_ecode	status;

	if (!args)
		return (FAILURE);
	if (!args[1])
		return (declare_builtin(shell));
	key = get_key_from_keyvalue(args[1]);
	if (!key)
		return (MALLOC_ERROR);
	value = NULL;
	status = get_value_from_keyvalue(args[1], &value);
	if (status != SUCCESS && status != NULL_STRING)
		return (ft_free((void **) &key), MALLOC_ERROR);
	update_env_node(&shell->env_list, key, value, true);
	return (SUCCESS);
}
