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





