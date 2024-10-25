#include "../../../include/minishell.h"

void	free_token(t_token **token)
{
	if (!token || !*token)
		return ;
	if ((*token)->str)
	{
		fprintf(stderr, "string to be freed: %s\n", (*token)->str);
		free((*token)->str);
		(*token)->str = NULL;
	}
	fprintf(stderr, "token to be freed: %p\n", (*token));
	free(*token);
	*token = NULL;
}

void	free_token_list(t_token **token_list)
{
	t_token	*next;

	if (!token_list || !(*token_list))
		return ;
	print_token(*token_list);
	while (*token_list)
	{
		next = (*token_list)->next;
		free_token(token_list);
		*token_list = next;
	}
}
