#include "../../include/minishell.h"

/**
 * @brief Checks whether a string is a natural number.
 * 
 * A natural number is any positive or negative integer.
 * 
 * @param arg The string to be checked.
 * 
 * @return If the string contains nothing else than a natural number,
 * (it can contain a negative sign/dash as the first character)
 * then it returns true. Otherwise it returns false.
 */
bool	ft_is_natural(char *arg)
{
	int	i;

	if (!arg || !arg[0] || ((arg[0] == '-' || arg[0] == '+') && !arg[1]))
		return (false);
	i = 0;
	while (arg && arg[i])
	{
		if (i == 0 && (arg[i] == '-' || arg[i] == '+'))
			i++;
		if (!ft_isdigit(arg[i]))
			return (false);
		i++;
	}
	return (true);
}
