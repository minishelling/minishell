#include "../../../include/minishell.h"

void	free_tree(t_tree **node)
{
	if (*node == NULL)
		return ;
	free_tree(&(*node)->left);
	free_tree(&(*node)->right);
	if ((*node)->type == CMD)
		free_cmd(&(*node)->cmd);
	free(*node);
	*node = NULL;
}
