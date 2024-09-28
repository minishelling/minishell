#include "../../include/minishell.h"

static bool	is_valid_identifier(char *keyval);

t_ecode	export_builtin(t_shell *shell, char **cmd_args)
{
	char	*key;
	char	*value;
	t_ecode	status;
	char	*error_message;

	//printf("We're running export\n");
	if (!cmd_args)
		return (FAILURE);
	if (!cmd_args[1])
		return (declare_builtin(shell, cmd_args));
	if (!is_valid_identifier(cmd_args[1]))
	{
		error_message = ft_strjoin("mini_shared: export: '", cmd_args[1]); //Protect...
		error_message = ft_strjoin_fs1(&error_message, "': not a valid identifier\n"); //Protect... Handle error separately.
		write (STDOUT_FILENO, error_message, ft_strlen(error_message));
		return (ft_free((void **) &error_message), FAILURE);
	}
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

static bool	is_valid_identifier(char *keyval)
{
	int	i;

	if (keyval && (keyval[0] != '_' && !ft_isalpha(keyval[0])))
		return (false);
	i = 1;
	while (keyval[i])
	{
		if (keyval[i] == '=')
			break ;
		if (keyval[i] != '_' && !ft_isalnum(keyval[i]))
			return (false);
		i++;
	}
	return (true);	
}
