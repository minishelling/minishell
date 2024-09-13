#include "../../include/minishell.h"

int echo_builtin(t_cmd *cmd) //Return unsigned int8_t?
{
	if (ft_strncmp(cmd->args[0], "echo", 4))
		printf("The command is not echo, why did you call this function?\n");
	return (0);
}
