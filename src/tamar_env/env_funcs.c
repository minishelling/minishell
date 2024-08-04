#include "../../include/minishell.h"

//char	*env_get_value(char *keyvalue)
char	*env_get_value_from_key(t_env *env_head, char *key)
{
	t_env *cur;
	
	if ( !env_head || !key || !key[0])
		return (NULL);

	printf ("key is now %s\n", key);
	cur = env_head;
	while (cur)
	{
		if (ft_strncmp (key, cur->key, ft_strlen(cur->key)) == 0)
			return (cur->value);
		cur = cur ->next;
	}
	return (NULL);
}
