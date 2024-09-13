#include "../../../include/minishell.h"

int parser_pipe(t_cmd *cmd_node, t_token *token)
{
	(void) cmd_node;
	(void) token;
	return (0);
}

t_redir_id	redir_ident(char *str)
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
	return (0);
}

int parser_redir(t_cmd *cmd, t_token *token)
{
	t_token	*file_token;
	t_redir	*redir_list;

	file_token = get_after_space_token(token);
	redir_list = new_redir();
	if (!redir_list)
	{
		cmd->next = (t_cmd *) 0XFF; // ?
		return (0);
	}
	redir_list->redir_id = redir_ident(token->str);
	redir_list->file = file_token->str;
	if (redir_list->file[0] == '|' || redir_list->file[0] == '&' || redir_list->file[0] == ';'
		|| redir_list->file[0] == '(' || redir_list->file[0] == ')')
			return (ERR_SYNTAX_ERROR);
	add_redir_in_back(&(cmd->redir), redir_list);
	return (0);
}

int parser_word(t_cmd *cmd, t_token *token)
{
	size_t	i;
	char	**arr;

	arr = cmd->args;
	i = 0;
	while (arr[i] != NULL)
		i++;
	arr[i] = token->str;
	
	return (0);
	
}

int	parser_space(t_cmd *cmd, t_token *token)
{
	(void) cmd;
	(void)token;
	return (0);
}

int	parser_semicol(t_cmd *cmd_node, t_token *token)
{
	(void) cmd_node;
	(void) token;
	return (0);
}

int	parser_and_opr(t_cmd *cmd_node, t_token *token)
{
	(void) cmd_node;
	(void) token;
	return (0);
}

int parser_par_close(t_cmd *cmd_node, t_token *token)
{
	(void) cmd_node;
	(void) token;
	return (0);
}

int parser_par_open(t_cmd *cmd_node, t_token *token)
{
	(void) cmd_node;
	(void) token;
	return (0L);
}

int parser_or_opr(t_cmd *cmd_node, t_token *token)
{
	(void) cmd_node;
	(void) token;
	return (0);
}

int parser_arith_expan(t_cmd *cmd_node, t_token *token)
{
	(void) token;
    cmd_node->args = ft_calloc(2, sizeof(char *));
    cmd_node->args[0] = ft_strdup("((");
    cmd_node->args[1] = NULL;
	return (0);
}