#include "../../include/minishell.h"

t_ecode	unset_builtin(t_shell *shell, char **cmd_args)
{
	char	*key;
	t_env 	*env_node;

	if (!shell || !cmd_args)
		return (NULL_ERROR);
	if (!cmd_args[1])
		return (SUCCESS);
	key = cmd_args[1];
	env_node = find_env_node(shell->env_list, key);
	if (!env_node)
		return (NULL_NODE);
	return (free_env_node(&env_node), SUCCESS);
}
