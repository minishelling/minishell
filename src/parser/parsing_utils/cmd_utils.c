#include "../../../include/minishell.h"

t_cmd	*new_cmd(void)
{
	t_cmd	*cur_cmd;

	cur_cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	if (!cur_cmd)
		return (NULL);
	cur_cmd->latest_in = STDIN_FILENO;
	cur_cmd->latest_out = STDOUT_FILENO;
	return (cur_cmd);
}
