#include "../../include/minishell.h"

bool	ft_is_natural(char *arg)
{
	int	i;

	i = 0;
	while (arg && arg[i])
	{
		if (i == 0 && arg[i] == '-')
			i++;
		if (!ft_isdigit(arg[i]))
			return (false);
		i++;
	}
	return (true);
}
