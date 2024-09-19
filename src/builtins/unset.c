#include "../../include/minishell.h"

t_ecode	unset_builtin(t_shell *shell, char *key)
{
	t_env *env_node;

	env_node = find_env_node(shell->env_list, key);
	if (!env_node)
		return (NULL_NODE);
	return (free_env_node(&env_node));
}
