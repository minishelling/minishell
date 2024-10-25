#include "../../../include/minishell.h"

void free_token(t_token **token)
{
	if (token && *token)
	{
		if ((*token)->str)
		{
			free((*token)->str);
			(*token)->str = NULL;
		}
		else
		{
			free(*token);
			*token = NULL;
		}
	}
}


// void	free_token(t_token **token)
// {
// 	if (!token && !*token)
// 	return ;
// 	if ((*token)->str)
// 	{
// 		free((*token)->str);
// 		(*token)->str = NULL;
// 	}
// 	free(*token);
// 	*token = NULL;
// }

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
