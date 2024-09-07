#include "../../include/minishell.h"

void free_tree(t_tree *node)
{
	if (node == NULL)
		return;
	free_tree(node->left);
	free_tree(node->right);

	// if (node->cmd_list)
	//     free_cmd_list(node->cmd_list);   //todo
	free(node);
}
