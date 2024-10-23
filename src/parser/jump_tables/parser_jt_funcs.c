#include "../../../include/minishell.h"

int parser_noop(t_cmd *cmd_node, t_token *token)
{
	(void)cmd_node;
	(void)token;
	return (PARSING_OK);
}

t_redir_id	which_redir(char *str)
{
	if (str == NULL)
		return (0);
	if (str[0] == '<')
	{
		if (str[1] == '<')
			return (HERE);
		return (IN);
	}
	if (str[0] == '>')
	{
		if (str[1] == '>')
			return (APP);
		return (OUT);
	}
	return (REDIR);
}

int parser_redir(t_cmd *cmd, t_token *token)
{
	t_token	*file_token;
	t_redir	*redir_list;

	if (token->next)
		file_token = token->next;
	// else
	// 	return(ERR_SYNTAX_ERROR);  //syntax erro - no file name?
	redir_list = new_redir();
	if (!redir_list)
		return (ERR_MEM);
	redir_list->redir_id = which_redir(token->str);
	redir_list->file = ft_strdup(file_token->str);
	if (!redir_list->file)
		return (ERR_MEM);
	if (redir_list->file[0] == '|' || redir_list->file[0] == '&' || redir_list->file[0] == ';'
		|| redir_list->file[0] == '(' || redir_list->file[0] == ')')
			return (ERR_SYNTAX_ERROR);
	add_redir_in_back(&(cmd->redir), redir_list);
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

int parser_arith_expan(t_cmd *cmd_node, t_token *token)
{
	(void)token;
	if (cmd_node->args)
	{
		free_args(&cmd_node->args);
		cmd_node->args = NULL;
	}
	cmd_node->args = ft_calloc(3, sizeof(char *));
	cmd_node->args[0] = ft_strdup("((");
	if (!cmd_node->args[0])
		return (ERR_MEM);
	cmd_node->args[1] = ft_strdup("))");
	if (!cmd_node->args[1])
	cmd_node->args[2] = NULL;
		return (ERR_MEM);
	return (PARSING_OK);
}