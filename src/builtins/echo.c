#include "../../include/minishell.h"

t_ecode echo_builtin(t_shell *shell, char **cmd_args)
{
	bool	newline;
	char	**str_arr;
	int		i;
	int		j;

	(void) shell;
	if (!cmd_args)
		return (FAILURE);
	if (!cmd_args[1])
	{
		ft_putchar_fd('\n', 1);
		return (SUCCESS);
	}
	if (cmd_args[1] && !ft_strncmp(cmd_args[1], "-n", 2))
	{
		newline = false;
		str_arr = &cmd_args[2];
	}
	else
	{
		newline = true;
		str_arr = &cmd_args[1];
	}
	i = 0;
	while (str_arr[i])
	{
		j = 0;
		while (str_arr[i][j])
		{
			if (str_arr[i][j] != '\\')
				ft_putchar_fd(str_arr[i][j], STDOUT_FILENO);
			j++;
		}
		if (str_arr[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (SUCCESS);
}
