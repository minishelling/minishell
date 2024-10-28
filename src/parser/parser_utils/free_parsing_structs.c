#include "../../../include/minishell.h"

void	free_token_list(t_token **list);
void	free_token(t_token **token);
void	free_cmd(t_cmd **cmd);
void	free_args(char ***args);
void	free_redir_list(t_redir **list);

void	free_token_list(t_token **list)
{
	t_token	*next;

	if (!list || !(*list))
		return ;
	while (*list)
	{
		next = (*list)->next;
		free_token(list);
		*list = next;
	}
}

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

void	free_redir_list(t_redir **list)
{
	t_redir	*temp_redir;

	if (!list || !(*list))
		return ;
	while (*list)
	{
		temp_redir = *list;
		*list = (*list)->next;
		if (temp_redir->file)
		{
			free(temp_redir->file);
			temp_redir->file = NULL;
		}
		free(temp_redir);
	}
	*list = NULL;
}
