#include "../../../../include/minishell.h"

int	parser_noop(t_cmd *cmd_node, t_token *token)
{
	(void)cmd_node;
	(void)token;
	return (PARSING_OK);
}

int	parser_add_new_arg(t_cmd *cmd, t_token *token)
{
	size_t	i;

	i = 0;
	while (cmd->args[i])
		i++;
	cmd->args[i] = ft_strdup(token->str);
	if (!cmd->args[i])
		return (ERR_MEM);
	return (PARSING_OK);
}
