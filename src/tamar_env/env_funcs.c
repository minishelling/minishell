#include "../../include/minishell.h"

char *get_env_value_from_key(t_env *env_head, char *token_key) 
{
    t_env *cur;
    size_t token_key_len;

    if (!env_head || !token_key || !token_key[0])
        return (NULL);
    token_key_len = ft_strlen(token_key);
    cur = env_head;
    while (cur) {
        if (token_key_len == ft_strlen(cur->key) &&
            ft_strncmp(token_key, cur->key, token_key_len) == 0) 
            	return cur->value;
        cur = cur->next;
    }
    return (NULL);
}

t_env	*init_env_var(void)
{
	t_env	*env_node;

	env_node = malloc(sizeof(t_env));
	if (!env_node)
		return (NULL);
	env_node->key = NULL;
	env_node->value = NULL;
	env_node->next = NULL;
	return (env_node);
}

void	add_env_var_in_back(t_env **env_list, t_env *new_env_var)
{
	t_env	*current;

	if (env_list == NULL || new_env_var == NULL)
		return ;
	if (*env_list == NULL)
	{
		*env_list = new_env_var;
		return ;
	}
	current = *env_list;
	while (current->next != NULL)
		current = current->next;
	current->next = new_env_var;
}

void	free_env_node(t_env *env_var_node)
{
	free(env_var_node->key);
	if (env_var_node->value)
		free(env_var_node->value);
	free(env_var_node);
}

void	free_env_list(t_env *env_list)
{
	t_env	*tmp_env;

	if (env_list == NULL)
		return ;
	while (env_list)
	{
		tmp_env = env_list->next;
		free_env_node(env_list);
		env_list = tmp_env;
	}
}



