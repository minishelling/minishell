#include "../../include/minishell.h"

int echo_builtin(char **args) //Return unsigned int8_t?
{
	bool	newline;
	char	**str_arr;
	int		i;

	if (!args)
		return (FAILURE);
	if (!args[1])
	{
		ft_putchar_fd('\n', 1);
		return (SUCCESS);
	}
	if (args[1] && !ft_strncmp(args[1], "-n", 2))
		{
			newline = false;
			str_arr = &args[2];
		}
	else
	{
		newline = true;
		str_arr = &args[1];
	}
	i = 0;
	while (str_arr[i])
	{
		ft_putstr_fd(str_arr[i], 1);
		if (str_arr[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', 1);
	return (SUCCESS);
}
