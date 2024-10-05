#include "../../include/minishell.h"

size_t	ft_str_count(char **cmd_args)
{
	size_t	count;

	if (!cmd_args || !*cmd_args)
		return (0);
	count = 0;
	while (cmd_args[count])
		count++;
	return (count);
}
