#include "../../include/minishell.h"

t_ecode	unset_builtin(t_shell *shell, char *key)
{
	t_env *env_node;

	env_node = env_find_node(shell->env_list, key);
	if (!env_node)
		return (NODE_NOT_FOUND);
	return (env_free_node(&env_node));
}
