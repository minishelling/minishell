#include "../../../include/minishell.h"

void	free_token(t_token **token)
{
	if (!token || !*token)
		return ;
	if ((*token)->str)
	{
		free((*token)->str);
		(*token)->str = NULL;
	}
	free(*token);
	*token = NULL;
}

void	free_token_list(t_token **token_list)
{
	t_token	*next;

	if (!token_list || !(*token_list))
		return ;
	while (*token_list)
	{
		next = (*token_list)->next;
		free_token(token_list);
		*token_list = next;
	}
}

void	free_redir_list(t_redir **redir)
{
	t_redir	*temp_redir;

	if (!redir || !(*redir))
		return ;
	while (*redir)
	{
		temp_redir = *redir;
		*redir = (*redir)->next;
		if (temp_redir->file)
		{
			free(temp_redir->file);
			temp_redir->file = NULL;
		}
		free(temp_redir);
	}
	*redir = NULL;
}

void	free_args(char ***args)
{
	int	i;

	if (!(*args))
		return ;
	i = 0;
	while ((*args)[i])
	{
		free((*args)[i]);
		(*args)[i] = NULL;
		i++;
	}
	free(*args);
	*args = NULL;
}

void	free_cmd(t_cmd **cmd)
{
	if (*cmd == NULL)
		return ;
	if (cmd && *cmd && (*cmd)->args)
		free_args(&(*cmd)->args);
	if ((*cmd)->redir)
		free_redir_list(&(*cmd)->redir);
	free(*cmd);
	*cmd = NULL;
}
