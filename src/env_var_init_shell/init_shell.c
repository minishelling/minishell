#include "../../include/minishell.h"

int	unset_env_var(char *key, t_env **env_var_list)  //builtin
{
	int		len_key;
	t_env	*current;
	t_env	*temp_var;

	if (!key)
		return (-2); //malloc error
	current = *env_var_list;
	temp_var = NULL;
	len_key = ft_strlen(key);
	while (current != NULL && current->next != NULL)
	{
		if (ft_strncmp(current->next->key, key, len_key) == 0)
		{
			temp_var = current->next;
			current->next = temp_var->next;
			env_var_free_node(temp_var);
			return (0); //SUCCESS
		}
		current = current->next;
	}
	return (0); // variable name not found thus not unset! // SUCCESS
}

int	init_shell_update_SHLVL(t_env **env_var_list)
{
	t_env	*shlvl_node;
	unsigned int	value;

	shlvl_node = env_var_get_env_node("SHLVL", *env_var_list);
	if (shlvl_node)
	{
		value = ft_atoi(shlvl_node->value) + 1;
		if (shlvl_node->value)
			free(shlvl_node->value);
		shlvl_node->value = ft_itoa(value);
		if (!shlvl_node->value)
			return (1);
		return (0);
	}
	shlvl_node = env_var_create_new_node("SHLVL=1");
	if (!shlvl_node)
		return (1);
	env_var_add_to_end_list(env_var_list, shlvl_node);
	return (0);
}

int	init_shell(char **envp, t_shell *shell)
{
	t_env	*env_var_list;
	t_env	*new_env_var;
	int				i;

	env_var_list = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		new_env_var = env_var_create_new_node(envp[i]);
		if (!new_env_var)
			return (env_var_free_list(env_var_list), 1);
		env_var_add_to_end_list(&env_var_list, new_env_var);
		i++;
	}
	if (init_shell_update_SHLVL(&env_var_list))
		return (env_var_free_list(env_var_list), 1);
	unset_env_var("OLDPWD", &env_var_list);
	shell->env_list = env_var_list;
	return (0);
}


//--------------------------


// char	*env_get_value(char *keyvalue)
// {
// 	char	*value;
// 	int		i;
// 	int		j;
// 	int		value_len;
	
// 	if (!keyvalue || !keyvalue[0])
// 		return (NULL);
// 	while (*keyvalue && *keyvalue != '=')
// 		keyvalue[i++];
// 	value_len = ft_strlen(keyvalue);
// 	value = ft_calloc(value_len, sizeof(char));
// 	if (!value)
// 		return (NULL);
// 	j = 0;
// 	keyvalue[i++];
// 	while (j < i)
// 	{
// 		value[j] = keyvalue[i + j];
// 		j++;
// 	}
// 	value[j] = '\0';
// 	return (value);
// }

// char	*env_get_key(char *keyvalue)
// {
// 	char	*key;
// 	int		i;
// 	int		j;

// 	if (!keyvalue || !keyvalue[0])
// 		return (NULL);
// 	i = 0;
// 	while (keyvalue[i] && keyvalue[i] != '=')
// 		i++;
// 	key = ft_calloc(i + 1, sizeof(char));
// 	if (!key)
// 		return (NULL);
// 	j = 0;
// 	while (j < i)
// 	{
// 		key[j] = keyvalue[j];
// 		j++;
// 	}
// 	key[j] = '\0';
// 	return (key);
// }


// t_env_ecode	env_copy_keyval(t_env **new_node, char *keyvalue)
// {
// 	(*new_node)->keyvalue = ft_strdup(keyvalue);
// 	if (!(*new_node)->keyvalue)
// 		return (ENV_GETKEYVALUE_ERROR);
// 	(*new_node)->key = env_get_key((*new_node)->keyvalue);
// 	if (!(*new_node)->key)
// 		return (ENV_GETKEY_ERROR);
// 	(*new_node)->value = env_get_value((*new_node)->keyvalue);
// 	if (!(*new_node)->value)
// 		return (ENV_GETVALUE_ERROR);
// 	(*new_node)->next = NULL;
// 	return (ENV_SUCCESS);
// }

// t_env	*env_new_node(void)
// {
// 	t_env	*new_node;
	
// 	new_node = ft_calloc(1, sizeof(t_env));
// 	if (!new_node)
// 		return (NULL);
// 	new_node->keyvalue = NULL;
// 	new_node->key = NULL;
// 	new_node->value = NULL;
// 	return (new_node);
// }

// ssize_t	env_count_keys(char **envp)
// {
// 	int	i;

// 	if (!envp || !*envp)
// 		return (-1);
// 	i = 0;
// 	while (envp[i])
// 		i++;
// 	return (i);
// }

// static t_env	*env_new_copied_node(char *keyvalue, t_env_ecode *status)
// {
// 	t_env	*new_node;

// 	*status = ENV_SUCCESS;
// 	new_node = env_new_node();
// 	if (!new_node)
// 	{
// 		*status = ENV_NEW_NODE_ERROR;
// 		return (NULL);
// 	}
// 	*status = env_copy_keyval(&new_node, keyvalue);
// 	if (*status != ENV_SUCCESS)
// 		return (NULL);
// 	return (new_node);
// }

// t_env_ecode	env_init_list(t_env **head, char **envp)
// {
// 	t_env		*current;
// 	t_env		*new_node;
// 	t_env_ecode	status;	
// 	size_t		i;

// 	if (!envp || !*envp)
// 		return (ENV_NULL);
// 	i = 0;
// 	while (envp[i])
// 	{
// 		new_node = env_new_copied_node(envp[i], &status);
// 		if (status != ENV_SUCCESS)
// 			return (status);
// 		if (!*head)
// 		{
// 			*head = new_node;
// 			current = *head;
// 		}
// 		else
// 		{
// 			current->next = new_node;
// 			current = current->next;
// 		}
// 		i++;
// 	}
// }