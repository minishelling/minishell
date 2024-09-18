#include "../../include/minishell.h"

int	export_builtin(t_shell *shell, char **args)
{
	t_env	*node;
	char	*key;
	char	*value;
	t_ecode	status;

	if (!args)
		return (FAILURE);
	if (!args[1])
		return (declare_builtin(shell));
	key = env_get_key(args[1]);
	if (!key)
		return (MALLOC_ERROR);
	status = env_get_value(args[1]);
	if (!value)
		return (MALLOC_ERROR);
	env_update_node(shell->env_list, )

	return (SUCCESS);
}
