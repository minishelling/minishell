#include "../../../include/minishell.h"

t_token	*parser_pipe(t_cmd *cmd_node, t_token *token)
{
	(void) cmd_node;
	(void) token;
	return (NULL);
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

t_token	*parser_redir(t_cmd *cmd, t_token *token)
{
	t_token	*t_file;
	t_redir	*redir;

	t_file = get_after_space_token(token);
	redir = new_redir();
	if (!redir)
	{
		cmd->next = (t_cmd *) 0XFF; //??
		return (NULL);
	}
	redir->redir = redir_ident(token->str);
	redir->file = t_file->str;
	add_redir_in_back(&(cmd->redir), redir);
	return (get_after_space_token(t_file));
}

t_token	*parser_word(t_cmd *cmd, t_token *token)
{
	size_t	i;
	char	**arr;

	arr = cmd->args;
	i = 0;
	while (arr[i] != NULL)
		i++;
	arr[i] = token->str;
	return (get_after_space_token(token));
}

t_token	*parser_space(t_cmd *cmd, t_token *token)
{
	(void) cmd;
	return (token->next);
}

t_token	*parser_semicol(t_cmd *cmd_node, t_token *token)
{
	(void) cmd_node;
	(void) token;
	return (NULL);
}

t_token	*parser_and_opr(t_cmd *cmd_node, t_token *token)
{
	(void) cmd_node;
	(void) token;
	return (NULL);
}

t_token	*parser_par_close(t_cmd *cmd_node, t_token *token)
{
	(void) cmd_node;
	(void) token;
	return (NULL);
}

t_token	*parser_par_open(t_cmd *cmd_node, t_token *token)
{
	(void) cmd_node;
	(void) token;
	return (NULL);
}

t_token	*parser_or_opr(t_cmd *cmd_node, t_token *token)
{
	(void) cmd_node;
	(void) token;
	return (NULL);
}